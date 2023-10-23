import { CFunction } from './cvizzu.types'
import { Events as E, Plugins } from './types/vizzu.js'

import { CChart, CEvent } from './module/cchart.js'
import { Render } from './render.js'

type EventRecord<Handler> = [CFunction | null, Handler[]]

interface EventState {
  canceled: boolean
}

type EventHandlers<T extends E.Type> = {
  [key in T]?: EventRecord<E.Handler<E.EventMap[T]>>
}

export class Events {
  private _cChart: CChart
  private _render: Render
  private _eventHandlers: EventHandlers<E.Type> = {}

  constructor(cChart: CChart, render: Render) {
    this._cChart = cChart
    this._render = render
  }

  add<T extends E.Type>(eventName: T, handler: E.Handler<E.EventMap[T]>): void {
    if (typeof eventName !== 'string') {
      throw new Error('first parameter should be string')
    }
    this._getHandlers(eventName).push(handler)
  }

  private _getHandlers<T extends E.Type>(eventName: T): E.Handler<E.EventMap[T]>[] {
    if (!(eventName in this._eventHandlers)) {
      let cfunc = null
      if (!this._isJSEvent(eventName)) {
        const func = (eventPtr: CEvent, param: E.EventMap[T]): void => {
          this._invoke(eventName, param, eventPtr)
        }
        cfunc = this._cChart.addEventListener(eventName, func)
      }
      this._eventHandlers[eventName] = [cfunc, []]
    }
    const handlers = this._eventHandlers[eventName]
    return handlers![1]
  }

  remove<T extends E.Type>(eventName: T, handler: E.Handler<E.EventMap[T]>): void {
    if (typeof eventName !== 'string') {
      throw new Error('first parameter should be string')
    }

    const eventRecord = this._eventHandlers[eventName]
    if (!eventRecord) throw new Error('unknown event handler')

    const [cfunc, handlers] = eventRecord

    handlers.find((o, i) => {
      if (o === handler) {
        handlers.splice(i, 1)
        return true
      }
      return false
    })

    if (handlers.length === 0) {
      if (!this._isJSEvent(eventName)) {
        if (cfunc) this._cChart.removeEventListener(eventName, cfunc)
      }
      delete this._eventHandlers[eventName]
    }
  }

  addMany(events: Plugins.PluginListeners): void {
    for (const [eventName, handler] of Object.entries(events)) {
      this.add(eventName as E.Type, handler as E.Handler<E.EventMap[E.Type]>)
    }
  }

  removeMany(events: Plugins.PluginListeners): void {
    for (const [eventName, handler] of Object.entries(events)) {
      this.remove(eventName as E.Type, handler as E.Handler<E.EventMap[E.Type]>)
    }
  }

  private _invoke<T extends E.Type>(eventName: T, param: E.EventMap[T], cEvent?: CEvent): boolean {
    const state: EventState = { canceled: false }
    try {
      const handlers = this._eventHandlers[eventName]
      if (handlers) {
        for (const handler of [...handlers[1]]) {
          const eventParam = this._isJSEvent(eventName)
            ? this._makeJSEventParam(param, state)
            : cEvent
            ? this._makeCEventParam(cEvent, param, state)
            : param
          handler(eventParam)
        }
      }
    } catch (e) {
      console.log('exception in event handler: ' + e)
    }
    return state.canceled
  }

  private _isJSEvent(eventName: E.Type): boolean {
    return eventName.startsWith('api-')
  }

  private _makeJSEventParam<T>(param: E.Event<T>, state: EventState): E.Event<T> {
    param.preventDefault = (): void => {
      state.canceled = true
    }
    return param
  }

  private _makeCEventParam<T>(cEvent: CEvent, param: E.Event<T>, state: EventState): E.Event<T> {
    param.preventDefault = (): void => {
      cEvent.preventDefault()
      state.canceled = true
    }
    if (param.type.endsWith('-draw') || param.type.startsWith('draw-')) {
      param.renderingContext = this._render.dc()
    }
    return param
  }
}

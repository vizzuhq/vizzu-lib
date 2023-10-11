/// <reference types="./types/vizzu.d.ts" />

import { CChart, CEvent } from "./module/cchart"
import { Render } from "./render"

type EventRecord<Handler> = [
  CFunction<(eventPtr: CEventPtr, param: CString) => void> | null,
  Handler[]
]

interface EventState {
  canceled: boolean
}

type EventHandlers<T extends Events.Type> = {
  [key in T]?: EventRecord<Events.Handler<Events.EventMap[T]>>
};

export class Events {
  _cChart: CChart
  _render: Render
  _eventHandlers: EventHandlers<Events.Type> = {}

  constructor(cChart: CChart, render: Render) {
    this._cChart = cChart
    this._render = render
  }

  add<T extends Events.Type>(eventName: T, handler: Events.Handler<Events.EventMap[T]>) {
    if (typeof eventName !== 'string') {
      throw new Error('first parameter should be string')
    }
    this._getHandlers(eventName).push(handler)
  }

  _getHandlers<T extends Events.Type>(eventName: T): Events.Handler<Events.EventMap[T]>[] {
    if (!(eventName in this._eventHandlers)) {
      let cfunc = null
      if (!this._isJSEvent(eventName)) {
        const func = (eventPtr: CEvent, param: Events.EventMap[T]) => {
          this._invoke(eventName, param, eventPtr)
        }
        cfunc = this._cChart.addEventListener(eventName, func)
      }
      this._eventHandlers[eventName] = [cfunc, []]
    }
    const handlers = this._eventHandlers[eventName]
    return handlers![1]
  }

  remove<T extends Events.Type>(eventName: T, handler: Events.Handler<Events.EventMap[T]>) {
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

  addMany(events: Plugins.PluginListeners) {
    for (const [eventName, handler] of Object.entries(events)) {
      this.add(eventName as Events.Type, handler)
    }
  }

  removeMany(events: Plugins.PluginListeners) {
    for (const [eventName, handler] of Object.entries(events)) {
      this.remove(eventName as Events.Type, handler)
    }
  }

  _invoke<T extends Events.Type>(eventName: T, param: Events.EventMap[T], cEvent?: CEvent) {
    const state: EventState = { canceled: false }
    try {
      const handlers = this._eventHandlers[eventName]
      if (handlers) {
        for (const handler of [...handlers[1]]) {
          const eventParam = this._isJSEvent(eventName)
            ? this._makeJSEventParam(param, state)
            : cEvent
              ? this._makeCEventParam(cEvent, param, state) : param
          handler(eventParam)
        }
      }
    } catch (e) {
      console.log('exception in event handler: ' + e)
    }
    return state.canceled
  }

  _isJSEvent(eventName: Events.Type) {
    return eventName.startsWith('api-')
  }

  _makeJSEventParam<T>(param: Events.Event<T>, state: EventState) {
    param.preventDefault = () => {
      state.canceled = true
    }
    return param
  }

  _makeCEventParam<T>(cEvent: CEvent, param: Events.Event<T>, state: EventState) {
    param.preventDefault = () => {
      cEvent.preventDefault()
      state.canceled = true
    }
    if (param.type.endsWith('-draw') || param.type.startsWith('draw-')) {
      param.renderingContext = this._render.dc()
    }
    return param
  }
}

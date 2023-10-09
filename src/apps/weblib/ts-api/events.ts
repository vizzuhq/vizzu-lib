/// <reference types="./types/vizzu.d.ts" />

import { CChart, CEvent } from "./module/cchart"
import { Render } from "./render"

type EventRecord = [
  CFunction<(eventPtr: CEventPtr, param: CString) => void> | null,
  Event.Handler[]
]

interface EventState {
  canceled: boolean
}

export class Events {
  _cChart: CChart
  _render: Render
  _eventHandlers: Map<Event.Type, EventRecord>

  constructor(cChart: CChart, render: Render) {
    this._cChart = cChart
    this._render = render
    this._eventHandlers = new Map<Event.Type, EventRecord>()
  }

  add(eventName: Event.Type, handler: Event.Handler) {
    if (typeof eventName !== 'string') {
      throw new Error('first parameter should be string')
    }

    if (!this._eventHandlers.has(eventName)) {
      let cfunc = null
      if (!this._isJSEvent(eventName)) {
        const func = (eventPtr: CEvent, param: Event.Event) => {
          this._invoke(eventName, param, eventPtr)
        }
        cfunc = this._cChart.addEventListener(eventName, func)
      }
      this._eventHandlers.set(eventName, [cfunc, []])
    }
    this._eventHandlers.get(eventName)![1].push(handler)
  }

  remove(eventName: Event.Type, handler: Event.Handler) {
    if (typeof eventName !== 'string') {
      throw new Error('first parameter should be string')
    }

    const eventRecord = this._eventHandlers.get(eventName)
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
      this._eventHandlers.delete(eventName)
    }
  }

  addMany(events: Plugins.PluginListeners) {
    for (const [eventName, handler] of Object.entries(events)) {
      this.add(eventName as Event.Type, handler)
    }
  }

  removeMany(events: Plugins.PluginListeners) {
    for (const [eventName, handler] of Object.entries(events)) {
      this.remove(eventName as Event.Type, handler)
    }
  }

  _invoke(eventName: Event.Type, param: Event.Event, cEvent?: CEvent) {
    const state: EventState = { canceled: false }
    try {
      const handlers = this._eventHandlers.get(eventName)
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

  _isJSEvent(eventName: Event.Type) {
    return eventName.startsWith('api-')
  }

  _makeJSEventParam(param: Event.Event, state: EventState) {
    param.preventDefault = () => {
      state.canceled = true
    }
    return param
  }

  _makeCEventParam(cEvent: CEvent, param: Event.Event, state: EventState) {
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

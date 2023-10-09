export class Events {
  constructor(vizzu) {
    this.vizzu = vizzu
    this.eventHandlers = new Map()
  }

  add(eventName, handler) {
    if (eventName !== '' + eventName) {
      throw new Error('first parameter should be string')
    }

    if (!this.eventHandlers.has(eventName)) {
      let cfunc = null
      if (!this._isJSEvent(eventName)) {
        const func = (eventPtr, param) => {
          this._invoke(eventName, param, eventPtr)
        }
        cfunc = this.vizzu._cChart.addEventListener(eventName, func)
      }
      this.eventHandlers.set(eventName, [cfunc, []])
    }
    this.eventHandlers.get(eventName)[1].push(handler)
  }

  remove(eventName, handler) {
    if (eventName !== '' + eventName) {
      throw new Error('first parameter should be string')
    }

    if (!this.eventHandlers.has(eventName)) throw new Error('unknown event handler')

    const [cfunc, handlers] = this.eventHandlers.get(eventName)

    handlers.find((o, i) => {
      if (o === handler) {
        handlers.splice(i, 1)
        return true
      }
      return false
    })

    if (handlers.length === 0) {
      if (!this._isJSEvent(eventName)) {
        this.vizzu._cChart.removeEventListener(eventName, cfunc)
      }
      this.eventHandlers.delete(eventName)
    }
  }

  addMany(events) {
    for (const [eventName, handler] of Object.entries(events)) {
      this.add(eventName, handler)
    }
  }

  removeMany(events) {
    for (const [eventName, handler] of Object.entries(events)) {
      this.remove(eventName, handler)
    }
  }

  _invoke(eventName, param, cEvent = null) {
    const state = { canceled: false }
    try {
      if (this.eventHandlers.has(eventName)) {
        for (const handler of [...this.eventHandlers.get(eventName)[1]]) {
          const eventParam = this._isJSEvent(eventName)
            ? this._makeJSEventParam(param, state)
            : this._makeCEventParam(cEvent, param, state)
          handler(eventParam)
        }
      }
    } catch (e) {
      console.log('exception in event handler: ' + e)
    }
    return state.canceled
  }

  _isJSEvent(eventName) {
    return eventName.startsWith('api-')
  }

  _makeJSEventParam(param, state) {
    param.preventDefault = () => {
      state.canceled = true
    }
    return param
  }

  _makeCEventParam(cEvent, param, state) {
    param.preventDefault = () => {
      cEvent.preventDefault()
      state.canceled = true
    }
    if (param.data?.markerId) {
      param.data.getMarker = this._getMarkerProxy(param.data.markerId)
    }
    if (param.type.endsWith('-draw') || param.type.startsWith('draw-')) {
      param.renderingContext = this.vizzu.render.dc()
    }
    return param
  }

  _getMarkerProxy(markerId) {
    let markerData = null
    return () => {
      if (!markerData) {
        markerData = this._cChart.getMarkerData(markerId)
      }
      return markerData
    }
  }
}

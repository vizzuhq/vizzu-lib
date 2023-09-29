export default class Events {
  constructor(vizzu) {
    this.vizzu = vizzu
    this.module = vizzu.module
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
        cfunc = this.module.addFunction(func, 'vii')
        const cname = this.vizzu._toCString(eventName)
        try {
          this.vizzu._callOnChart(this.module._addEventListener)(cname, cfunc)
        } finally {
          this.module._free(cname)
        }
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
        const cname = this.vizzu._toCString(eventName)
        try {
          this.vizzu._callOnChart(this.module._removeEventListener)(cname, cfunc)
        } finally {
          this.module._free(cname)
        }
        this.module.removeFunction(cfunc)
      }
      this.eventHandlers.delete(eventName)
    }
  }

  _invoke(eventName, param, eventPtr = 0) {
    const state = { canceled: false }
    try {
      if (this.eventHandlers.has(eventName)) {
        for (const handler of [...this.eventHandlers.get(eventName)[1]]) {
          const eventParam = this._isJSEvent(eventName)
            ? this._makeJSEventParam(param, state)
            : this._makeCEventParam(eventPtr, param, state)
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

  _makeCEventParam(eventPtr, cString, state) {
    const json = this.vizzu._fromCString(cString)
    const param = JSON.parse(json)
    param.preventDefault = () => {
      this.vizzu._call(this.module._event_preventDefault)(eventPtr)
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
        const cStr = this.vizzu._callOnChart(this.vizzu.module._chart_markerData)(markerId)
        markerData = JSON.parse(this.vizzu.module.UTF8ToString(cStr))
      }
      return markerData
    }
  }
}

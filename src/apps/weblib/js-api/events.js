export default class Events {
  constructor(vizzu) {
    this.vizzu = vizzu
    this.module = vizzu.module
    this.eventHandlers = new Map()
  }

  _isJSEvent(eventName) {
    return eventName.startsWith('api-')
  }

  add(eventName, handler) {
    if (eventName !== '' + eventName) {
      throw new Error('first parameter should be string')
    }

    if (!this.eventHandlers.has(eventName)) {
      let cfunc = null
      if (!this._isJSEvent(eventName)) {
        const func = (param) => {
          this._invoke(eventName, param)
        }
        cfunc = this.module.addFunction(func, 'vi')
        const cname = this.vizzu._toCString(eventName)
        try {
          this.vizzu._call(this.module._addEventListener)(cname, cfunc)
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
          this.vizzu._call(this.module._removeEventListener)(cname, cfunc)
        } finally {
          this.module._free(cname)
        }
        this.module.removeFunction(cfunc)
      }
      this.eventHandlers.delete(eventName)
    }
  }

  _invoke(eventName, param) {
    let canceled = false
    try {
      if (this.eventHandlers.has(eventName)) {
        let eventParam
        if (!this._isJSEvent(eventName)) {
          const jsparam = this.vizzu._fromCString(param)
          eventParam = JSON.parse(jsparam)
          eventParam.preventDefault = () => {
            this.vizzu._call(this.module._event_preventDefault)()
          }
          if (eventParam.data?.markerId) {
            eventParam.data.getMarker = this._getMarkerProxy(eventParam.data.markerId)
          }
          if (eventParam.type.endsWith('-draw') || eventParam.type.startsWith('draw-')) {
            eventParam.renderingContext = this.vizzu.render.dc()
          }
        } else {
          eventParam = param
          eventParam.preventDefault = () => {
            canceled = true
          }
        }
        for (const handler of [...this.eventHandlers.get(eventName)[1]]) {
          handler(eventParam)
        }
      }
    } catch (e) {
      console.log('exception in event handler: ' + e)
    }
    return canceled
  }

  _getMarkerProxy(markerId) {
    let markerData = null
    return () => {
      if (!markerData) {
        const cStr = this.vizzu._call(this.vizzu.module._chart_markerData)(markerId)
        markerData = JSON.parse(this.vizzu.module.UTF8ToString(cStr))
      }
      return markerData
    }
  }
}

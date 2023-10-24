export var EventType
;(function (EventType) {
  EventType['click'] = 'click'
  EventType['pointeron'] = 'pointeron'
  EventType['pointermove'] = 'pointermove'
  EventType['pointerdown'] = 'pointerdown'
  EventType['pointerup'] = 'pointerup'
  EventType['wheel'] = 'wheel'
  EventType['update'] = 'update'
  EventType['drawBegin'] = 'draw-begin'
  EventType['drawComplete'] = 'draw-complete'
  EventType['backgroundDraw'] = 'background-draw'
  EventType['titleDraw'] = 'title-draw'
  EventType['subtitleDraw'] = 'subtitle-draw'
  EventType['captionDraw'] = 'caption-draw'
  EventType['logoDraw'] = 'logo-draw'
  EventType['legendBackgroundDraw'] = 'legend-background-draw'
  EventType['legendTitleDraw'] = 'legend-title-draw'
  EventType['legendLabelDraw'] = 'legend-label-draw'
  EventType['legendMarkerDraw'] = 'legend-marker-draw'
  EventType['legendBarDraw'] = 'legend-bar-draw'
  EventType['plotBackgroundDraw'] = 'plot-background-draw'
  EventType['plotAreaDraw'] = 'plot-area-draw'
  EventType['plotMarkerDraw'] = 'plot-marker-draw'
  EventType['plotMarkerLabelDraw'] = 'plot-marker-label-draw'
  EventType['plotMarkerGuideDraw'] = 'plot-marker-guide-draw'
  EventType['plotAxisDraw'] = 'plot-axis-draw'
  EventType['plotAxisTitleDraw'] = 'plot-axis-title-draw'
  EventType['plotAxisLabelDraw'] = 'plot-axis-label-draw'
  EventType['plotAxisTickDraw'] = 'plot-axis-tick-draw'
  EventType['plotAxisGuideDraw'] = 'plot-axis-guide-draw'
  EventType['plotAxisInterlacingDraw'] = 'plot-axis-interlacing-draw'
  EventType['animationBegin'] = 'animation-begin'
  EventType['animationComplete'] = 'animation-complete'
})(EventType || (EventType = {}))
export class Events {
  constructor(cChart, render) {
    this._eventHandlers = {}
    this._cChart = cChart
    this._render = render
  }
  add(eventName, handler) {
    if (typeof eventName !== 'string') {
      throw new Error('first parameter should be string')
    }
    this._getHandlers(eventName).push(handler)
  }
  _getHandlers(eventName) {
    if (!(eventName in this._eventHandlers)) {
      let cfunc = null
      if (!this._isJSEvent(eventName)) {
        const func = (eventPtr, param) => {
          this._invoke(eventName, param, eventPtr)
        }
        cfunc = this._cChart.addEventListener(eventName, func)
      }
      this._eventHandlers[eventName] = [cfunc, []]
    }
    const handlers = this._eventHandlers[eventName]
    return handlers[1]
  }
  remove(eventName, handler) {
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
  _invoke(eventName, param, cEvent) {
    const state = { canceled: false }
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
    if (param.type.endsWith('-draw') || param.type.startsWith('draw-')) {
      param.renderingContext = this._render.dc()
    }
    return param
  }
}

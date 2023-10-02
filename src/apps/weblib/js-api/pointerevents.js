export default class PointerEvents {
  meta = {
    name: 'pointerEvents'
  }

  register(chart) {
    this._chart = chart
  }

  unregister() {
    if (this._enabled) this._removeHandlers()
  }

  enabled(enabled) {
    this._enabled = enabled
    if (enabled) this._setHandlers()
    else this._removeHandlers()
  }

  _handlerList = ['pointermove', 'pointerup', 'pointerdown', 'pointerleave', 'wheel']

  _setHandlers() {
    this._canvas = this._chart.getCanvasElement()
    for (const handler of this._handlerList) {
      this[`_${handler}Handler`] = this[`_${handler}`].bind(this)
      this._canvas.addEventListener(handler, this[`_${handler}Handler`])
    }
  }

  _removeHandlers() {
    for (const handler of this._handlerList) {
      if (this[`_${handler}Handler`]) {
        this._canvas.removeEventListener(handler, this[`_${handler}Handler`])
        delete this[`_${handler}Handler`]
      }
    }
    delete this._canvas
  }

  _pointermove(evt) {
    this._passEventToChart(evt, this._chart.module._vizzu_pointerMove)
  }

  _pointerup(evt) {
    this._passEventToChart(evt, this._chart.module._vizzu_pointerUp)
  }

  _pointerdown(evt) {
    this._passEventToChart(evt, this._chart.module._vizzu_pointerDown)
  }

  _pointerleave(evt) {
    this._passEventToChart(evt, this._chart.module._vizzu_pointerLeave)
  }

  _wheel(evt) {
    this._passEventToChart(evt, this._chart.module._vizzu_wheel, 'deltaY')
  }

  _passEventToChart(evt, cfunc, member = 'pointerId') {
    const args = [this._chart.render.ccanvas.id, evt[member]]
    if (evt.clientX !== undefined) {
      const clientPos = { x: evt.clientX, y: evt.clientY }
      const pos = this._chart.render.clientToRenderCoor(clientPos)
      args.push(pos.x, pos.y)
    }
    this._chart._callOnChart(cfunc)(...args)
  }
}

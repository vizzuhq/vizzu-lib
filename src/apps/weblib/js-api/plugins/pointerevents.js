import { NotInitializedError } from '../errors.js'
export class PointerEvents {
  constructor(cChart) {
    this._enabled = false
    this.meta = {
      name: 'pointerEvents'
    }
    this._handlers = {
      pointermove: (evt) => {
        this._passEventToChart(evt, cChart.pointermove.bind(cChart))
      },
      pointerup: (evt) => {
        this._passEventToChart(evt, cChart.pointerup.bind(cChart))
      },
      pointerdown: (evt) => {
        this._passEventToChart(evt, cChart.pointerdown.bind(cChart))
      },
      pointerleave: (evt) => {
        this._passEventToChart(evt, cChart.pointerleave.bind(cChart))
      },
      wheel: (evt) => {
        this._passEventToChart(evt, cChart.wheel.bind(cChart), 'deltaY')
      }
    }
  }
  register(vizzu) {
    this._vizzu = vizzu
    this._rendering = this._vizzu?.feature('rendering')
  }
  unregister() {
    if (this._enabled) this._removeHandlers()
  }
  enable(enabled) {
    this._enabled = enabled
    if (enabled) this._setHandlers()
    else this._removeHandlers()
  }
  _setHandlers() {
    if (!this._vizzu || !this._handlers) {
      throw new NotInitializedError()
    }
    this._canvas = this._vizzu.getCanvasElement()
    for (const [key, value] of Object.entries(this._handlers)) {
      this._canvas.addEventListener(key, value)
    }
  }
  _removeHandlers() {
    if (this._handlers && this._canvas) {
      for (const [key, value] of Object.entries(this._handlers)) {
        this._canvas.removeEventListener(key, value)
      }
    }
    delete this._canvas
  }
  _passEventToChart(evt, func, member = 'pointerId') {
    const args = [evt[member]]
    if (evt.clientX !== undefined) {
      args.push(...this._getCoords(evt))
    }
    func(...args)
  }
  _getCoords(evt) {
    const clientPos = { x: evt.clientX, y: evt.clientY }
    if (!this._rendering) throw new NotInitializedError()
    const pos = this._rendering.clientToRenderCoor(clientPos)
    return [pos.x, pos.y]
  }
}

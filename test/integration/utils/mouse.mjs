export class Mouse {
  constructor(chart) {
    this._chart = chart._chart
  }

  down(x, y) {
    this._chart._cChart._call(this._chart._cChart._wasm._vizzu_pointerDown)(
      this._chart._render._ccanvas.getId(),
      0,
      x,
      y
    )
    return this
  }

  move(x, y) {
    this._chart._cChart._call(this._chart._cChart._wasm._vizzu_pointerMove)(
      this._chart._render._ccanvas.getId(),
      0,
      x,
      y
    )
    return this
  }

  up(x, y) {
    this._chart._cChart._call(this._chart._cChart._wasm._vizzu_pointerUp)(
      this._chart._render._ccanvas.getId(),
      0,
      x,
      y
    )
    return this
  }

  wheel(delta) {
    this._chart._cChart._call(this._chart._cChart._wasm._vizzu_wheel)(
      this._chart._render._ccanvas.getId(),
      delta
    )
    return this
  }
}

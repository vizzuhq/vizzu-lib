/// <reference types="../types/vizzu.d.ts" />

import Vizzu from '../vizzu'
import { NotInitializedError } from '../errors'

export class PointerEvents implements Plugins.Plugin {
  _vizzu?: Vizzu
  _canvas?: HTMLCanvasElement
  _handlers = {}
  _enabled = false
  _handlerList = ['pointermove', 'pointerup', 'pointerdown', 'pointerleave', 'wheel']

  meta = {
    name: 'pointerEvents'
  }

  register(vizzu: Vizzu) {
    this._vizzu = vizzu
  }

  unregister() {
    if (this._enabled) this._removeHandlers()
  }

  enable(enabled: boolean) {
    this._enabled = enabled
    if (enabled) this._setHandlers()
    else this._removeHandlers()
  }

  _setHandlers() {
    this._canvas = this._vizzu!.getCanvasElement()
    for (const handlerName of this._handlerList) {
      ;(this._handlers as any)[handlerName] = (this as any)[handlerName].bind(this)
      this._canvas.addEventListener(handlerName, (this._handlers as any)[handlerName])
    }
  }

  _removeHandlers() {
    for (const handlerName of this._handlerList) {
      if ((this._handlers as any)[handlerName]) {
        this._canvas?.removeEventListener(handlerName, (this._handlers as any)[handlerName])
        delete (this._handlers as any)[handlerName]
      }
    }
    delete this._canvas
  }

  pointermove(evt: PointerEvent) {
    this._passEventToChart(evt, this._wasm()._vizzu_pointerMove)
  }

  pointerup(evt: PointerEvent) {
    this._passEventToChart(evt, this._wasm()._vizzu_pointerUp)
  }

  pointerdown(evt: PointerEvent) {
    this._passEventToChart(evt, this._wasm()._vizzu_pointerDown)
  }

  pointerleave(evt: PointerEvent) {
    this._passEventToChart(evt, this._wasm()._vizzu_pointerLeave)
  }

  wheel(evt: WheelEvent) {
    this._passEventToChart(evt, this._wasm()._vizzu_wheel, 'deltaY')
  }

  _chart() {
    if (!this._vizzu || !this._vizzu._chart) throw new NotInitializedError()
    return this._vizzu._chart
  }

  _wasm() {
    return this._chart()._module._wasm
  }

  _render() {
    return this._chart()._render
  }

  _passEventToChart<T extends MouseEvent, F>(evt: T, cfunc: F, member = 'pointerId') {
    const args = [this._render()._ccanvas.getId(), evt[member as keyof T]]
    if (evt.clientX !== undefined) {
      const clientPos = { x: evt.clientX, y: evt.clientY }
      const pos = this._render().clientToRenderCoor(clientPos)
      args.push(pos.x, pos.y)
    }
    this._chart()._cChart._call(cfunc as any)(...args)
  }
}

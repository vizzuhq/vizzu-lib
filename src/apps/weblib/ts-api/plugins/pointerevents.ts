import { Plugins } from '../types/vizzu.js'

import Vizzu from '../vizzu.js'
import { Chart } from '../chart.js'
import { Render } from '../render.js'
import { CVizzu } from '../cvizzu.types.js'
import { NotInitializedError } from '../errors.js'

interface Handlers {
  pointermove: (event: PointerEvent) => void
  pointerup: (event: PointerEvent) => void
  pointerdown: (event: PointerEvent) => void
  pointerleave: (event: PointerEvent) => void
  wheel: (event: WheelEvent) => void
}

export class PointerEvents implements Plugins.Plugin {
  _vizzu?: Vizzu
  _canvas?: HTMLCanvasElement
  _handlers?: Handlers
  _enabled = false

  meta = {
    name: 'pointerEvents'
  }

  register(vizzu: Vizzu): void {
    this._vizzu = vizzu

    this._handlers = {
      pointermove: (evt: PointerEvent): void => {
        this._passEventToChart(evt, this._wasm()._vizzu_pointerMove)
      },
      pointerup: (evt: PointerEvent): void => {
        this._passEventToChart(evt, this._wasm()._vizzu_pointerUp)
      },
      pointerdown: (evt: PointerEvent): void => {
        this._passEventToChart(evt, this._wasm()._vizzu_pointerDown)
      },
      pointerleave: (evt: PointerEvent): void => {
        this._passEventToChart(evt, this._wasm()._vizzu_pointerLeave)
      },
      wheel: (evt: WheelEvent): void => {
        this._passEventToChart(evt, this._wasm()._vizzu_wheel, 'deltaY')
      }
    }
  }

  unregister(): void {
    if (this._enabled) this._removeHandlers()
  }

  enable(enabled: boolean): void {
    this._enabled = enabled
    if (enabled) this._setHandlers()
    else this._removeHandlers()
  }

  _setHandlers(): void {
    if (!this._vizzu || !this._handlers) {
      throw new NotInitializedError()
    }

    this._canvas = this._vizzu.getCanvasElement()
    for (const [key, value] of Object.entries(this._handlers)) {
      this._canvas.addEventListener(key, value)
    }
  }

  _removeHandlers(): void {
    if (this._handlers && this._canvas) {
      for (const [key, value] of Object.entries(this._handlers)) {
        this._canvas.removeEventListener(key, value)
      }
    }
    delete this._canvas
  }

  _chart(): Chart {
    if (!this._vizzu || !this._vizzu._chart) throw new NotInitializedError()
    return this._vizzu._chart
  }

  _wasm(): CVizzu {
    return this._chart()._module._wasm
  }

  _render(): Render {
    return this._chart()._render
  }

  _passEventToChart<T extends MouseEvent, F>(evt: T, cfunc: F, member = 'pointerId'): void {
    const args: unknown[] = [this._render()._ccanvas.getId(), evt[member as keyof T]]
    if (evt.clientX !== undefined) {
      args.push(...this._getCoords(evt))
    }
    this._chart()._cChart._call(cfunc as (cSelf: number, ...params: unknown[]) => unknown)(...args)
  }

  _getCoords(evt: MouseEvent): number[] {
    const clientPos = { x: evt.clientX, y: evt.clientY }
    const pos = this._render().clientToRenderCoor(clientPos)
    return [pos.x, pos.y]
  }
}

import { Plugins } from '../types/vizzu.js'

import Vizzu from '../vizzu.js'
import { Chart } from '../chart.js'
import { CChart } from '../module/cchart.js'
import { Render } from '../render.js'
import { NotInitializedError } from '../errors.js'

interface Handlers {
  pointermove: (event: PointerEvent) => void
  pointerup: (event: PointerEvent) => void
  pointerdown: (event: PointerEvent) => void
  pointerleave: (event: PointerEvent) => void
  wheel: (event: WheelEvent) => void
}

export class PointerEvents implements Plugins.Plugin {
  private _vizzu?: Vizzu
  private _canvas?: HTMLCanvasElement
  private _handlers?: Handlers
  private _enabled = false

  meta = {
    name: 'pointerEvents'
  }

  register(vizzu: Vizzu): void {
    this._vizzu = vizzu

    this._handlers = {
      pointermove: (evt: PointerEvent): void => {
        this._passEventToChart(evt, this._cChart().pointermove.bind(this._cChart()))
      },
      pointerup: (evt: PointerEvent): void => {
        this._passEventToChart(evt, this._cChart().pointerup.bind(this._cChart()))
      },
      pointerdown: (evt: PointerEvent): void => {
        this._passEventToChart(evt, this._cChart().pointerdown.bind(this._cChart()))
      },
      pointerleave: (evt: PointerEvent): void => {
        this._passEventToChart(evt, this._cChart().pointerleave.bind(this._cChart()))
      },
      wheel: (evt: WheelEvent): void => {
        this._passEventToChart(evt, this._cChart().wheel.bind(this._cChart()), 'deltaY')
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

  private _setHandlers(): void {
    if (!this._vizzu || !this._handlers) {
      throw new NotInitializedError()
    }

    this._canvas = this._vizzu.getCanvasElement()
    for (const [key, value] of Object.entries(this._handlers)) {
      this._canvas.addEventListener(key, value)
    }
  }

  private _removeHandlers(): void {
    if (this._handlers && this._canvas) {
      for (const [key, value] of Object.entries(this._handlers)) {
        this._canvas.removeEventListener(key, value)
      }
    }
    delete this._canvas
  }

  private _chart(): Chart {
    if (!this._vizzu || !this._vizzu._chart) throw new NotInitializedError()
    return this._vizzu._chart
  }

  private _cChart(): CChart {
    return this._chart()._cChart
  }

  private _render(): Render {
    return this._chart()._render
  }

  private _passEventToChart<T extends MouseEvent, F>(evt: T, func: F, member = 'pointerId'): void {
    const args: unknown[] = [this._render()._ccanvas, evt[member as keyof T]]
    if (evt.clientX !== undefined) {
      args.push(...this._getCoords(evt))
    }
    ;(func as (...params: unknown[]) => unknown)(...args)
  }

  private _getCoords(evt: MouseEvent): number[] {
    const clientPos = { x: evt.clientX, y: evt.clientY }
    const pos = this._render().clientToRenderCoor(clientPos)
    return [pos.x, pos.y]
  }
}

import { Plugins } from '../plugins'

import Vizzu from '../vizzu.js'
import { CChart } from '../module/cchart.js'
import { RenderingApi } from '../render.js'
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
  private _rendering?: RenderingApi
  private _canvas?: HTMLCanvasElement
  private _handlers?: Handlers
  private _enabled = false

  meta = {
    name: 'pointerEvents'
  }

  constructor(cChart: CChart) {
    this._handlers = {
      pointermove: (evt: PointerEvent): void => {
        this._passEventToChart(evt, cChart.pointermove.bind(cChart))
      },
      pointerup: (evt: PointerEvent): void => {
        this._passEventToChart(evt, cChart.pointerup.bind(cChart))
      },
      pointerdown: (evt: PointerEvent): void => {
        this._passEventToChart(evt, cChart.pointerdown.bind(cChart))
      },
      pointerleave: (evt: PointerEvent): void => {
        this._passEventToChart(evt, cChart.pointerleave.bind(cChart))
      },
      wheel: (evt: WheelEvent): void => {
        this._passEventToChart(evt, cChart.wheel.bind(cChart), 'deltaY')
      }
    }
  }

  register(vizzu: Vizzu): void {
    this._vizzu = vizzu

    this._rendering = this._vizzu?.feature('rendering') as RenderingApi
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

  private _passEventToChart<T extends MouseEvent, F>(evt: T, func: F, member = 'pointerId'): void {
    const args: unknown[] = [evt[member as keyof T]]
    if (evt.clientX !== undefined) {
      args.push(...this._getCoords(evt))
    }
    ;(func as (...params: unknown[]) => unknown)(...args)
  }

  private _getCoords(evt: MouseEvent): number[] {
    const clientPos = { x: evt.clientX, y: evt.clientY }
    if (!this._rendering) throw new NotInitializedError()
    const pos = this._rendering.clientToRenderCoor(clientPos)
    return [pos.x, pos.y]
  }
}

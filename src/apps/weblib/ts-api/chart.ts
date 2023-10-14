import { Anim, Geom } from './types/vizzu.js'
import { Module } from './module/module.js'
import { CChart, CAnimation, Snapshot } from './module/cchart.js'
import { CData } from './module/cdata.js'
import { Render } from './render.js'
import { Data } from './data.js'
import { Events } from './events.js'
import { Hooks, PluginRegistry } from './plugins.js'
import { Logging } from './plugins/logging.js'
import { Rendering } from './plugins/rendering.js'
import { Shorthands } from './plugins/shorthands.js'
import { PivotData } from './plugins/pivotdata.js'
import { Tooltip } from './plugins/tooltip.js'
import { PointerEvents } from './plugins/pointerevents.js'
import { CSSProperties } from './plugins/cssproperties.js'

export class Chart {
  _module: Module
  _container: HTMLElement
  _plugins: PluginRegistry
  _canvas: HTMLCanvasElement
  _cChart: CChart
  _cData: CData
  _render: Render
  _data: Data
  _events: Events
  _resizeObserver: ResizeObserver
  _updateInterval?: ReturnType<typeof setTimeout>

  constructor(module: Module, container: HTMLElement, _plugins: PluginRegistry) {
    this._plugins = _plugins
    this._container = container
    this._module = module

    this._canvas = this._createCanvas()

    const ccanvas = this._module.createCanvas()
    this._cChart = this._module.createChart()
    this._cData = this._module.getData(this._cChart)
    this._data = new Data(this._cData)
    this._render = new Render(ccanvas, this._cChart, this._canvas, false)
    this._module.registerRenderer(ccanvas, this._render)
    this._events = new Events(this._cChart, this._render)
    this._plugins.init(this._events)
    this._resizeObserver = this._createResizeObserverFor(this._canvas)
  }

  registerBuilts(): void {
    this._plugins.register(new Logging(), false)
    this._plugins.register(new Rendering(), true)
    this._plugins.register(new CSSProperties(), false)
    this._plugins.register(new Shorthands(), true)
    this._plugins.register(new PivotData(), true)
    this._plugins.register(new PointerEvents(), true)
    this._plugins.register(new Tooltip(), false)
  }

  _createResizeObserverFor(canvas: HTMLCanvasElement): ResizeObserver {
    const resizeObserver = new ResizeObserver(() => {
      this._render.updateFrame(true)
    })
    resizeObserver.observe(canvas)
    return resizeObserver
  }

  _createCanvas(): HTMLCanvasElement {
    let canvas = null
    const placeholder = this._container

    if (placeholder instanceof HTMLCanvasElement) {
      canvas = placeholder
    } else {
      canvas = document.createElement('CANVAS')
      canvas.style.width = '100%'
      canvas.style.height = '100%'
      placeholder.appendChild(canvas)
    }

    if (!(canvas instanceof HTMLCanvasElement)) {
      throw new Error('Error initializing <canvas> for Vizzu!')
    }

    return canvas
  }

  start(): void {
    if (!this._updateInterval) {
      this._render.updateFrame()
      this._updateInterval = setInterval(() => {
        this._render.updateFrame()
      }, 25)
    }
  }

  _animate(
    callback: (ok: boolean) => void,
    target: Anim.Keyframes | CAnimation,
    options?: Anim.ControlOptions & Anim.Options
  ): void {
    const ctx = Object.assign({ target }, options !== undefined ? { options } : {})
    this._plugins.hook(Hooks.setAnimParams, ctx).default((ctx) => {
      this._setAnimParams(ctx.target, ctx.options)
    })
    this._cChart.animate(callback)
  }

  _setAnimParams(
    target: Anim.Keyframes | CAnimation,
    options?: Anim.ControlOptions & Anim.Options
  ): void {
    if (target instanceof CAnimation) {
      this._cChart.restoreAnim(target)
    } else {
      for (const keyframe of target) this._setKeyframe(keyframe.target, keyframe.options)
    }
    if (options) this._cChart.animOptions.set(options)
  }

  _setKeyframe(target: Anim.Target | Snapshot, options?: Anim.Options): void {
    if (target) {
      if (target instanceof Snapshot) {
        this._cChart.restoreSnapshot(target)
      } else {
        if (target.data) this._data.set(target.data)
        if (target.style) this._cChart.style.set(target.style)
        if (target.config) this._cChart.config.set(target.config)
      }
    }
    if (options) this._cChart.animOptions.set(options)
    this._cChart.setKeyframe()
  }

  destruct(): void {
    this._resizeObserver.disconnect()
    if (this._updateInterval) clearInterval(this._updateInterval)
    delete this._updateInterval
    if (this._container && this._container !== this._canvas)
      this._container.removeChild(this._canvas)
  }

  getConverter(target: string, from: string, to: string): (point: Geom.Point) => Geom.Point {
    if (typeof target === 'string' && target.startsWith('plot')) {
      if (this._cChart) {
        const chart = this._cChart
        if (from === 'relative' || to === 'canvas')
          return (p: Geom.Point) => chart.toCanvasCoords(p)
        if (from === 'canvas' || to === 'relative') return (p: Geom.Point) => chart.toRelCoords(p)
      }
    }
    return (point: Geom.Point) => point
  }
}

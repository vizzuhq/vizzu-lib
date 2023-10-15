import { Anim, Config, Geom, Data as D, Styles, Events as E } from './types/vizzu.js'
import { Module } from './module/module.js'
import { CChart, Snapshot } from './module/cchart.js'
import { CAnimation } from './module/canimctrl.js'
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
import { CAnimControl } from './module/canimctrl.js'

export class Chart {
  _module: Module
  _cChart: CChart
  _render: Render
  private _container: HTMLElement
  private _canvas: HTMLCanvasElement
  private _cData: CData
  private _data: Data
  private _events: Events
  private _plugins: PluginRegistry
  private _resizeObserver: ResizeObserver
  private _updateInterval?: ReturnType<typeof setTimeout>

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

  private _createResizeObserverFor(canvas: HTMLCanvasElement): ResizeObserver {
    const resizeObserver = new ResizeObserver(() => {
      this._render.updateFrame(true)
    })
    resizeObserver.observe(canvas)
    return resizeObserver
  }

  private _createCanvas(): HTMLCanvasElement {
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

  animate(
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

  private _setAnimParams(
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

  private _setKeyframe(target: Anim.Target | Snapshot, options?: Anim.Options): void {
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

  getConverter(
    target: string,
    from: Geom.CoordinateType,
    to: Geom.CoordinateType
  ): (point: Geom.Point) => Geom.Point {
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

  version(): string {
    return this._module.version()
  }

  getCanvasElement(): HTMLCanvasElement {
    return this._canvas
  }

  forceUpdate(): void {
    this._render.updateFrame(true)
  }

  get data(): Readonly<D.Metainfo> {
    return this._cData.getMetaInfo()
  }

  get config(): Readonly<Config.Chart> {
    return this._cChart.config.get()
  }

  get style(): Readonly<Styles.Chart> {
    return this._cChart.style.get()
  }

  getComputedStyle(): Styles.Chart {
    return this._cChart.computedStyle.get()
  }

  on<T extends E.Type>(eventName: T, handler: E.Handler<E.EventMap[T]>): void {
    this._events.add(eventName, handler)
  }

  off<T extends E.Type>(eventName: T, handler: E.Handler<E.EventMap[T]>): void {
    this._events.remove(eventName, handler)
  }

  store(): Snapshot {
    return this._cChart.storeSnapshot()
  }

  getCAnimControl(): CAnimControl {
    return this._module.getAnimControl(this._cChart)
  }
}

import { Snapshot } from './module/cchart.js'
import { CAnimation } from './module/canimctrl.js'
import { Render } from './render.js'
import { Data } from './data.js'
import { Events } from './events.js'
import { Hooks } from './plugins.js'
import { Logging } from './plugins/logging.js'
import { Shorthands } from './plugins/shorthands.js'
import { PivotData } from './plugins/pivotdata.js'
import { Tooltip } from './plugins/tooltip.js'
import { PointerEvents } from './plugins/pointerevents.js'
import { CSSProperties } from './plugins/cssproperties.js'
export class Chart {
  constructor(module, container, _plugins) {
    this._plugins = _plugins
    this._container = container
    this._module = module
    this._canvas = this._createCanvas()
    this._cChart = this._module.createChart()
    this._cData = this._module.getData(this._cChart)
    this._data = new Data(this._cData)
    this._render = new Render(this._module, this._cChart, this._canvas, false)
    this._events = new Events(this._cChart, this._render)
    this._plugins.init(this._events)
    this._resizeObserver = this._createResizeObserverFor(this._canvas)
  }
  registerBuilts() {
    this._plugins.register(new Logging(this._module.setLogging.bind(this._module)), false)
    this._plugins.register(this._render, true)
    this._plugins.register(new CSSProperties(), false)
    this._plugins.register(new Shorthands(), true)
    this._plugins.register(new PivotData(), true)
    this._plugins.register(new PointerEvents(this._cChart), true)
    this._plugins.register(new Tooltip(), false)
  }
  _createResizeObserverFor(canvas) {
    const resizeObserver = new ResizeObserver(() => {
      this._render.updateFrame(true)
    })
    resizeObserver.observe(canvas)
    return resizeObserver
  }
  _createCanvas() {
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
  start() {
    if (!this._updateInterval) {
      this._render.updateFrame()
      this._updateInterval = setInterval(() => {
        this._render.updateFrame()
      }, 25)
    }
  }
  async prepareAnimation(target, options) {
    const ctx = Object.assign({ target }, options !== undefined ? { options } : {})
    await this._plugins.hook(Hooks.prepareAnimation, ctx).default((ctx) => {
      this.setAnimParams(ctx.target, ctx.options)
    })
  }
  runAnimation(callback) {
    const ctx = { callback }
    this._plugins.hook(Hooks.runAnimation, ctx).default((ctx) => {
      this._cChart.animate(ctx.callback)
    })
  }
  setAnimParams(target, options) {
    if (target instanceof CAnimation) {
      this._cChart.restoreAnim(target)
    } else {
      for (const keyframe of target) this._setKeyframe(keyframe.target, keyframe.options)
    }
    if (options) this._cChart.animOptions.set(options)
  }
  _setKeyframe(target, options) {
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
  destruct() {
    this._resizeObserver.disconnect()
    if (this._updateInterval) clearInterval(this._updateInterval)
    delete this._updateInterval
    if (this._container && this._container !== this._canvas)
      this._container.removeChild(this._canvas)
  }
  getConverter(target, from, to) {
    if (typeof target === 'string' && target.startsWith('plot')) {
      if (this._cChart) {
        const chart = this._cChart
        if (from === 'relative' || to === 'canvas') return (p) => chart.toCanvasCoords(p)
        if (from === 'canvas' || to === 'relative') return (p) => chart.toRelCoords(p)
      }
    }
    return (point) => point
  }
  version() {
    return this._module.version()
  }
  getCanvasElement() {
    return this._canvas
  }
  forceUpdate() {
    this._render.updateFrame(true)
  }
  get data() {
    return this._cData.getMetaInfo()
  }
  get config() {
    return this._cChart.config.get()
  }
  get style() {
    return this._cChart.style.get()
  }
  getComputedStyle() {
    return this._cChart.computedStyle.get()
  }
  on(eventName, handler) {
    this._events.add(eventName, handler)
  }
  off(eventName, handler) {
    this._events.remove(eventName, handler)
  }
  store() {
    return this._cChart.storeSnapshot()
  }
  getCAnimControl() {
    return this._module.getAnimControl(this._cChart)
  }
}

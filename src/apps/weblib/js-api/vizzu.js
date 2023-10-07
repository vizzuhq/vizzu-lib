import { loader } from './module/loader.js'
import { Animation, Snapshot } from './module/cchart.js'
import { CObject } from './module/cenv.js'
import { Rendering, Render } from './render.js'
import { Events } from './events.js'
import { Data } from './data.js'
import { AnimControl } from './animcontrol.js'
import { recursiveCopy, iterateObject, cloneObject } from './utils.js'
import { CancelError } from './errors.js'
import { Plugins } from './plugins.js'
import { Logging } from './plugins/logging.js'
import { Shorthands } from './plugins/shorthands.js'
import { PointerEvents } from './plugins/pointerevents.js'
import { PivotData } from './plugins/pivotdata.js'
import { Tooltip } from './plugins/tooltip.js'
import { CSSProperties } from './plugins/cssproperties.js'
import Presets from './plugins/presets.js'

class Hooks {
  static setAnimParams = 'setAnimParams'
  static animateRegister = 'animateRegister'
}

export default class Vizzu {
  static get presets() {
    return new Presets()
  }

  static options(options) {
    loader.options = options
  }

  static initialize() {
    return loader.initialize()
  }

  constructor(options, initState) {
    const opts = this._processOptions(options)

    this._container = opts.container

    this._started = false

    this._plugins = new Plugins(this, opts.features)

    this.initializing = loader.initialize().then((module) => {
      this._init(module)
      return this
    })

    this.anim = this.initializing

    if (initState) {
      this.initializing = this.animate(initState, 0)
    }
  }

  _processOptions(options) {
    const opts =
      typeof options !== 'object' || options instanceof HTMLElement
        ? { container: options }
        : options

    if (!('container' in opts)) {
      throw new Error('container not specified')
    }

    if (!(opts.container instanceof HTMLElement)) {
      opts.container = document.getElementById(opts.container)
    }

    if (!opts.container) {
      throw new Error(`Cannot find container ${opts.container} to render Vizzu!`)
    }

    return opts
  }

  get config() {
    this._validateModule()
    return cloneObject(
      this._cChart.listConfigParams.bind(this._cChart),
      this._cChart.getConfigParam.bind(this._cChart)
    )
  }

  get style() {
    this._validateModule()
    return cloneObject(
      this._cChart.listStyleParams.bind(this._cChart),
      this._cChart.getStyleParam.bind(this._cChart, false)
    )
  }

  getComputedStyle() {
    this._validateModule()
    return cloneObject(
      this._cChart.listStyleParams.bind(this._cChart),
      this._cChart.getStyleParam.bind(this._cChart, true)
    )
  }

  get data() {
    if (this._cChart) {
      return this._cData.getMetaInfo()
    }
  }

  on(eventName, handler) {
    this._validateModule()
    this.events.add(eventName, handler)
  }

  off(eventName, handler) {
    this._validateModule()
    this.events.remove(eventName, handler)
  }

  store() {
    if (this._cChart) {
      return this._cChart.storeSnapshot()
    }
  }

  get feature() {
    const fn = this._feature.bind(this)
    return new Proxy(fn, {
      get: (_target, pluginName) => {
        return this._plugins.api(pluginName)
      }
    })
  }

  _feature(nameOrInstance, enabled) {
    if (enabled !== undefined && typeof enabled !== 'boolean')
      throw new Error('enabled parameter must be boolean if specified')

    let name
    let enabledInRegister = false
    if (typeof nameOrInstance !== 'string') {
      const instance = nameOrInstance
      name = this._plugins.getRegisteredName(instance)
      if (!name) {
        name = this._plugins.register(instance, enabled || true)
        enabledInRegister = true
      }
    } else {
      name = nameOrInstance
    }

    const enabledProvided = enabled !== undefined
    if (enabledProvided && !enabledInRegister) {
      this._plugins.enable(name, enabled)
    }

    return this._plugins.api(name)
  }

  _validateModule() {
    if (!this._module) {
      throw new Error('Vizzu not initialized. Use `initializing` promise.')
    }
  }

  animate(target, options) {
    const copiedTarget = recursiveCopy(target, CObject)
    const copiedOptions = recursiveCopy(options)
    const ctx = { target: copiedTarget, options: copiedOptions, promise: this.anim }
    this._plugins.hook(Hooks.animateRegister, ctx).default((ctx) => {
      let activate
      const activated = new Promise((resolve, reject) => {
        activate = resolve
      })
      ctx.promise = ctx.promise.then(() => this._animate(copiedTarget, copiedOptions, activate))
      ctx.promise.activated = activated
    })
    this.anim = ctx.promise
    return this.anim
  }

  _animate(target, options, activate) {
    const anim = new Promise((resolve, reject) => {
      this._plugins.hook(Hooks.setAnimParams, { target, options }).default((ctx) => {
        this._processAnimParams(ctx.target, ctx.options)
      })
      const callback = (ok) => {
        if (ok) {
          resolve(this)
        } else {
          // eslint-disable-next-line prefer-promise-reject-errors
          reject(new CancelError())
          this.anim = Promise.resolve(this)
        }
      }
      this._cChart.animate(callback)
    }, this)
    activate(new AnimControl(this))
    return anim
  }

  _processAnimParams(target, options) {
    if (target instanceof Animation) {
      this._cChart.restoreAnim(target)
    } else {
      for (const keyframe of target) this._setKeyframe(keyframe.target, keyframe.options)
    }
    this._setAnimOptions(options)
  }

  _setKeyframe(target, options) {
    if (target) {
      if (target instanceof Snapshot) {
        this._cChart.restoreSnapshot(target)
      } else {
        this._data.set(target.data)
        this._setStyle(target.style)
        this._setConfig(target.config)
      }
    }
    this._setAnimOptions(options)
    this._cChart.setKeyframe()
  }

  _setConfig(config) {
    iterateObject(config, this._cChart.setConfigParam.bind(this._cChart))
  }

  _setStyle(style) {
    iterateObject(style, this._cChart.setStyleParam.bind(this._cChart))
  }

  _setAnimOptions(options) {
    if (typeof options !== 'undefined') {
      if (typeof options === 'object') {
        iterateObject(options, this._cChart.setAnimParam.bind(this._cChart))
      } else {
        throw new Error('invalid animation option')
      }
    }
  }

  // keeping this one for backward compatibility
  get animation() {
    return new AnimControl(this)
  }

  version() {
    this._validateModule()
    return this._module.version()
  }

  getCanvasElement() {
    return this.canvas
  }

  forceUpdate() {
    this._validateModule()
    this.render.updateFrame(true)
  }

  _start() {
    if (!this._started) {
      this.render.updateFrame()

      this._updateInterval = setInterval(() => {
        this.render.updateFrame()
      }, 25)

      this._started = true
    }
  }

  _init(module) {
    this._module = module
    this._module.setLogging(false)

    this.canvas = this._createCanvas()

    const ccanvas = this._module.createCanvas()
    this._cChart = this._module.createChart()
    this._cData = this._module.getData(this._cChart)

    this.render = new Render()
    this._data = new Data(this)
    this.events = new Events(this)
    this.render.init(ccanvas, this._cChart, this.canvas, false)
    this._module.registerRenderer(ccanvas, this.render)

    this._setupDOMEventHandlers(this.canvas)

    this.feature(new Logging(), false)
    this.feature(new CSSProperties(), false)
    this.feature(new Rendering(), true)
    this.feature(new Shorthands(), true)
    this.feature(new PivotData(), true)
    this.feature(new PointerEvents(), true)
    this.feature(new Tooltip(), false)

    this._start()
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

  _setupDOMEventHandlers(canvas) {
    this._resizeObserver = new ResizeObserver(() => {
      this.render.updateFrame(true)
    })
    this._resizeObserver.observe(canvas)
  }

  detach() {
    try {
      this._plugins.destruct()
    } finally {
      this._resizeObserver?.disconnect()
      if (this._updateInterval) clearInterval(this._updateInterval)
      if (this._container && this._container !== this.canvas)
        this._container.removeChild(this.canvas)
    }
  }

  getConverter(target, from, to) {
    if (typeof target === 'string' && target.startsWith('plot')) {
      if (this._cChart) {
        if (from === 'relative' || to === 'canvas') return (p) => this._cChart.toCanvasCoords(p)
        if (from === 'canvas' || to === 'relative') return (p) => this._cChart.toRelCoords(p)
      }
    }
    return (point) => point
  }
}

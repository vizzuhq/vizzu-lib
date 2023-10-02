import Render from './render.js'
import Events from './events.js'
import Data from './data.js'
import { Animation, AnimControl } from './animcontrol.js'
import Tooltip from './tooltip.js'
import Presets from './presets.js'
import VizzuModule from './cvizzu.js'
import CSSProperties from './cssproperties.js'
import ObjectRegistry from './objregistry.js'
import Plugins from './plugins.js'
import Shorthands from './shorthands.js'
import PointerEvents from './pointerevents.js'
import PivotData from './pivotdata.js'
import { recursiveCopy } from './utils.js'

class CancelError extends Error {
  constructor() {
    super('animation canceled')
    this.name = 'CancelError'
  }
}

class Hooks {
  static constructed = 'constructed'
  static setData = 'setData'
  static setConfig = 'setConfig'
  static setStyle = 'setStyle'
  static setAnimOptions = 'setAnimOptions'
  static animateRegister = 'animateRegister'
}

let vizzuOptions = null

class Snapshot {}

class Logging {
  meta = { name: 'logging' }

  register(chart) {
    this.chart = chart
  }

  enabled(enabled) {
    this.chart._validateModule()
    this.chart._call(this.chart.module._vizzu_setLogging)(enabled)
  }
}

class Rendering {
  meta = { name: 'rendering' }

  register(chart) {
    this.chart = chart
  }

  enabled(enabled) {
    this.chart._validateModule()
    this.chart.render.enabled = enabled
  }
}

class CChart {}

class CCanvas {}

export default class Vizzu {
  static get presets() {
    return new Presets()
  }

  static options(options) {
    vizzuOptions = options
  }

  constructor(options, initState) {
    const opts =
      typeof options !== 'object' || options instanceof HTMLElement
        ? { container: options }
        : options

    if (!('container' in opts)) {
      throw new Error('container not specified')
    }

    this._container = opts.container

    if (!(this._container instanceof HTMLElement)) {
      this._container = document.getElementById(opts.container)
    }

    if (!this._container) {
      throw new Error(`Cannot find container ${this._container} to render Vizzu!`)
    }

    this._plugins = new Plugins(this, opts.plugins)

    this._started = false

    this._resolveAnimate = null
    this.initializing = new Promise((resolve) => {
      this._resolveAnimate = resolve
    })
    this.anim = this.initializing

    const moduleOptions = {}

    if (vizzuOptions?.wasmUrl) {
      moduleOptions.locateFile = function (path) {
        if (path.endsWith('.wasm')) {
          return vizzuOptions.wasmUrl
        }
        return path
      }
    }

    // load module
    VizzuModule(moduleOptions).then((module) => {
      if (this._resolveAnimate) {
        this._resolveAnimate(this._init(module))
      }
    })

    if (initState) {
      this.initializing = this.animate(initState, 0)
    }

    this._plugins.hook(Hooks.constructed, this).default()
  }

  _callOnChart(f) {
    return this._call(f).bind(this, this._cChart.id)
  }

  _call(f) {
    return (...params) => {
      try {
        return f(...params)
      } catch (e) {
        if (Number.isInteger(e)) {
          const address = parseInt(e, 10)
          const type = new this.module.ExceptionInfo(address).get_type()
          const cMessage = this.module._vizzu_errorMessage(address, type)
          const message = this.module.UTF8ToString(cMessage)
          throw new Error('error: ' + message)
        } else {
          throw e
        }
      }
    }
  }

  _iterateObject(obj, paramHandler, path = '') {
    if (obj) {
      Object.keys(obj).forEach((key) => {
        const newPath = path + (path.length === 0 ? '' : '.') + key
        if (obj[key] !== null && typeof obj[key] === 'object') {
          this._iterateObject(obj[key], paramHandler, newPath)
        } else {
          this._setValue(newPath, obj[key], paramHandler)
        }
      })
    }
  }

  /* Note: If the value string containing a JSON, it will be parsed. */
  _setNestedProp(obj, path, value) {
    const propList = path.split('.')
    propList.forEach((prop, i) => {
      if (i < propList.length - 1) {
        obj[prop] = obj[prop] || (typeof propList[i + 1] === 'number' ? [] : {})
        obj = obj[prop]
      } else {
        obj[prop] = value.startsWith('[') || value.startsWith('{') ? JSON.parse(value) : value
      }
    })
  }

  _setValue(path, value, setter) {
    if (path !== '' + path) {
      throw new Error('first parameter should be string')
    }

    const cpath = this._toCString(path)
    const cvalue = this._toCString(String(value).toString())

    try {
      setter(cpath, cvalue)
    } finally {
      this.module._free(cvalue)
      this.module._free(cpath)
    }
  }

  _cloneObject(lister, getter, ...args) {
    const clistStr = this._call(lister)()
    const listStr = this._fromCString(clistStr)
    const list = JSON.parse(listStr)
    const res = {}
    for (const path of list) {
      const cpath = this._toCString(path)
      let cvalue
      try {
        cvalue = this._callOnChart(getter)(cpath, ...args)
        const value = this._fromCString(cvalue)
        this._setNestedProp(res, path, value)
      } finally {
        this.module._free(cpath)
      }
    }
    Object.freeze(res)
    return res
  }

  get config() {
    this._validateModule()
    return this._cloneObject(this.module._chart_getList, this.module._chart_getValue)
  }

  get style() {
    this._validateModule()
    return this._cloneObject(this.module._style_getList, this.module._style_getValue, false)
  }

  getComputedStyle() {
    this._validateModule()
    return this._cloneObject(this.module._style_getList, this.module._style_getValue, true)
  }

  get data() {
    this._validateModule()
    const cInfo = this._callOnChart(this.module._data_metaInfo)()
    const info = this._fromCString(cInfo)
    return { series: JSON.parse(info) }
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
    this._validateModule()
    return this._objectRegistry.get(this._callOnChart(this.module._chart_store), Snapshot)
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
    if (typeof nameOrInstance !== 'string') {
      name = this._plugins.getName(nameOrInstance)
      if (!name) {
        name = this._plugins.register(nameOrInstance, enabled || true)
      }
    } else name = nameOrInstance

    if (enabled !== undefined) {
      this._plugins.enabled(name, enabled)
    }
    return this._plugins.api(name)
  }

  _validateModule() {
    if (!this.module) {
      throw new Error('Vizzu not initialized. Use `initializing` promise.')
    }
  }

  animate(...args) {
    const copiedArgs = recursiveCopy(args)
    const ctx = { args: copiedArgs, promise: this.anim }
    this._plugins.hook(Hooks.animateRegister, ctx).default((ctx) => {
      let activate
      const activated = new Promise((resolve, reject) => {
        activate = resolve
      })
      ctx.promise = ctx.promise.then(() => this._animate(copiedArgs, activate))
      ctx.promise.activated = activated
    })
    this.anim = ctx.promise
    return this.anim
  }

  _animate(args, activate) {
    const anim = new Promise((resolve, reject) => {
      const callbackPtr = this.module.addFunction((ok) => {
        if (ok) {
          resolve(this)
        } else {
          // eslint-disable-next-line prefer-promise-reject-errors
          reject(new CancelError())
          this.anim = Promise.resolve(this)
        }
        this.module.removeFunction(callbackPtr)
      }, 'vi')
      this._processAnimParams(...args)
      this._callOnChart(this.module._chart_animate)(callbackPtr)
    }, this)
    activate(new AnimControl(this))
    return anim
  }

  _processAnimParams(animTarget, animOptions) {
    if (animTarget instanceof Animation) {
      this._callOnChart(this.module._chart_anim_restore)(animTarget.id)
    } else {
      const anims = []

      if (Array.isArray(animTarget)) {
        for (const target of animTarget)
          if (target.target !== undefined)
            anims.push({ target: target.target, options: target.options })
          else anims.push({ target, options: undefined })
      } else {
        anims.push({ target: animTarget, options: animOptions })
      }

      for (const anim of anims) this._setKeyframe(anim.target, anim.options)
    }
    this._setAnimOptions(animOptions)
  }

  _setKeyframe(animTarget, animOptions) {
    if (animTarget) {
      if (animTarget instanceof Snapshot) {
        this._callOnChart(this.module._chart_restore)(animTarget.id)
      } else {
        let obj = animTarget

        if (!obj.data && obj.style === undefined && !obj.config) {
          obj = { config: obj }
        }

        this._plugins.hook(Hooks.setData, { data: obj.data }).default((ctx) => {
          this._data.set(ctx.data)
        })

        if (obj.style === null) {
          obj.style = { '': null }
        }
        obj.style = obj.style || {}

        this._plugins.hook(Hooks.setStyle, { style: obj.style }).default((ctx) => {
          this._setStyle(ctx.style)
        })
        this._plugins.hook(Hooks.setConfig, { config: obj.config }).default((ctx) => {
          this._setConfig(ctx.config)
        })
      }
    }

    this._setAnimOptions(animOptions)

    this._callOnChart(this.module._chart_setKeyframe)()
  }

  _setConfig(config) {
    this._iterateObject(config, (path, value) => {
      this._callOnChart(this.module._chart_setValue)(path, value)
    })
  }

  _setStyle(style) {
    this._iterateObject(style, (path, value) => {
      this._callOnChart(this.module._style_setValue)(path, value)
    })
  }

  _setAnimOptions(animOptions) {
    this._plugins.hook(Hooks.setAnimOptions, { animOptions }).default((ctx) => {
      if (typeof ctx.animOptions !== 'undefined') {
        if (typeof ctx.animOptions === 'object') {
          this._iterateObject(ctx.animOptions, (path, value) => {
            this._callOnChart(this.module._anim_setValue)(path, value)
          })
        } else {
          throw new Error('invalid animation option')
        }
      }
    })
  }

  // keeping this one for backward compatibility
  get animation() {
    return new AnimControl(this)
  }

  version() {
    this._validateModule()
    return this.module.UTF8ToString(this.module._vizzu_version())
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

  _toCString(str) {
    const len = str.length * 4 + 1
    const buffer = this.module._malloc(len)
    this.module.stringToUTF8(str, buffer, len)
    return buffer
  }

  _fromCString(str) {
    return this.module.UTF8ToString(str)
  }

  _init(module) {
    this.module = module
    this.module.callback = this._call(this.module._callback)

    this.canvas = this._createCanvas()

    this.render = new Render()
    this._data = new Data(this)
    this.events = new Events(this)
    this.module.events = this.events
    this._objectRegistry = new ObjectRegistry(this._call(this.module._object_free))
    this._cChart = this._objectRegistry.get(this._call(this.module._vizzu_createChart), CChart)

    const ccanvas = this._objectRegistry.get(this._call(this.module._vizzu_createCanvas), CCanvas)
    this.render.init(ccanvas, this._callOnChart(this.module._vizzu_update), this.canvas, false)
    this.module.renders = this.module.renders || {}
    this.module.renders[ccanvas.id] = this.render

    this._call(this.module._vizzu_setLogging)(false)
    this._setupDOMEventHandlers(this.canvas)

    this.feature(new Logging(), false)
    this.feature(new Tooltip(), false)
    this.feature(new CSSProperties(), false)
    this.feature(new Rendering(), true)
    this.feature(new Shorthands(), true)
    this.feature(new PointerEvents(), true)
    this.feature(new PivotData(), true)

    this._start()

    return this
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
    this._plugins.destruct()
    this._resizeObserver?.disconnect()
    if (this._updateInterval) clearInterval(this._updateInterval)
    if (this._container && this._container !== this.canvas) this._container.removeChild(this.canvas)
  }

  getConverter(target, from, to) {
    this._validateModule()
    if (target === 'plot-area') {
      if (from === 'relative' || to === 'canvas') return this._toCanvasCoords.bind(this)
      if (from === 'canvas' || to === 'relative') return this._toRelCoords.bind(this)
    }
    return null
  }

  _toCanvasCoords(point) {
    const ptr = this._callOnChart(this.module._chart_relToCanvasCoords)(point.x, point.y)
    return {
      x: this.module.getValue(ptr, 'double'),
      y: this.module.getValue(ptr + 8, 'double')
    }
  }

  _toRelCoords(point) {
    const ptr = this._callOnChart(this.module._chart_canvasToRelCoords)(point.x, point.y)
    return {
      x: this.module.getValue(ptr, 'double'),
      y: this.module.getValue(ptr + 8, 'double')
    }
  }
}

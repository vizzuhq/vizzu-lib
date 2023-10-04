import Render from './render.js'
import Events from './events.js'
import Data from './data.js'
import { Animation, AnimControl } from './animcontrol.js'
import Tooltip from './tooltip.js'
import Presets from './presets.js'
import VizzuModule from './cvizzu.js'
import CSSProperties from './cssproperties.js'
import ObjectRegistry, { CObject } from './objregistry.js'
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
  static setAnimParams = 'setAnimParams'
  static animateRegister = 'animateRegister'
}

let vizzuOptions = null

class Snapshot extends CObject {}

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

class CChart extends CObject {}

class CCanvas extends CObject {}

export default class Vizzu {
  static get presets() {
    return new Presets()
  }

  static options(options) {
    vizzuOptions = options
  }

  constructor(options, initState) {
    const opts = this._processOptions(options)

    this._container = opts.container

    this._plugins = new Plugins(this, opts.features)

    this._started = false

    this.initializing = this._loadModule().then((module) => {
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

  _loadModule() {
    const moduleOptions = {}

    if (vizzuOptions?.wasmUrl) {
      moduleOptions.locateFile = function (path) {
        if (path.endsWith('.wasm')) {
          return vizzuOptions.wasmUrl
        }
        return path
      }
    }

    return VizzuModule(moduleOptions)
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
    const enabledProvided = enabled !== undefined
    let enabledSet = false
    if (typeof nameOrInstance !== 'string') {
      const instance = nameOrInstance
      name = this._plugins.getName(instance)
      const exists = name !== undefined
      if (!exists) {
        name = this._plugins.register(instance, enabled || true)
        enabledSet = true
      }
    } else name = nameOrInstance

    if (enabledProvided && !enabledSet) {
      this._plugins.enabled(name, enabled)
    }

    return this._plugins.api(name)
  }

  _validateModule() {
    if (!this.module) {
      throw new Error('Vizzu not initialized. Use `initializing` promise.')
    }
  }

  animate(target, options) {
    const copiedTarget = recursiveCopy(target)
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

      this._plugins.hook(Hooks.setAnimParams, { target, options }).default((ctx) => {
        this._processAnimParams(ctx.target, ctx.options)
      })
      this._callOnChart(this.module._chart_animate)(callbackPtr)
    }, this)
    activate(new AnimControl(this))
    return anim
  }

  _processAnimParams(target, options) {
    if (target instanceof Animation) {
      this._callOnChart(this.module._chart_anim_restore)(target.id)
    } else {
      for (const keyframe of target) this._setKeyframe(keyframe.target, keyframe.options)
    }
    this._setAnimOptions(options)
  }

  _setKeyframe(target, options) {
    if (target) {
      if (target instanceof Snapshot) {
        this._callOnChart(this.module._chart_restore)(target.id)
      } else {
        this._data.set(target.data)
        this._setStyle(target.style)
        this._setConfig(target.config)
      }
    }
    this._setAnimOptions(options)
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

  _setAnimOptions(options) {
    if (typeof options !== 'undefined') {
      if (typeof options === 'object') {
        this._iterateObject(options, (path, value) => {
          this._callOnChart(this.module._anim_setValue)(path, value)
        })
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
    this._call(this.module._vizzu_setLogging)(false)

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

    this._setupDOMEventHandlers(this.canvas)

    this.feature(new Logging(), false)
    this.feature(new Tooltip(), false)
    this.feature(new CSSProperties(), false)
    this.feature(new Rendering(), true)
    this.feature(new Shorthands(), true)
    this.feature(new PointerEvents(), true)
    this.feature(new PivotData(), true)

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

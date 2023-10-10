/// <reference types="./types/vizzu.d.ts" />

import { loader } from './module/loader'
import { Chart } from './chart'
import { CAnimation } from './module/cchart'
import { CObject } from './module/cenv'
import { AnimControl } from './animcontrol'
import { recursiveCopy } from './utils'
import { NotInitializedError, CancelError } from './errors'
import { Hooks, PluginRegistry } from './plugins'
/*
import { Logging } from './plugins/logging.js'
import { Shorthands } from './plugins/shorthands.js'
import { PointerEvents } from './plugins/pointerevents.js'
import { PivotData } from './plugins/pivotdata.js'
import { Tooltip } from './plugins/tooltip.js'
import { CSSProperties } from './plugins/cssproperties.js'
import Presets from './plugins/presets.js'
*/

export default class Vizzu implements Vizzu {
  _chart?: Chart
  initializing: Promise<Vizzu>
  _anim: Anim.Completing
  _container: HTMLElement
  _plugins: PluginRegistry
  /*
    static get presets() {
      return new Presets()
    }
  */
  static options(options: Lib.Options) {
    loader.options = options
  }

  static initialize() {
    return loader.initialize()
  }

  constructor(options: string | HTMLElement | VizzuOptions, initState?: Anim.Keyframes) {
    const opts = this._processOptions(options)

    this._container = opts.container

    this._plugins = new PluginRegistry(this, opts.features)

    this.initializing = loader.initialize().then((module) => {
      this._chart = new Chart(module, this._container, this._plugins)
      return this
    })

    this._anim = Object.assign(this.initializing, { activated: Promise.reject() })

    if (initState) {
      this.initializing = this.animate(initState, 0).then(() => this)
    }
  }

  _processOptions(options: any): VizzuOptions {
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

  get feature() {
    const fn = this._feature.bind(this)
    return new Proxy(fn, {
      get: (_target, pluginName: string) => {
        return this._plugins.api(pluginName)
      }
    })
  }

  _feature(nameOrInstance: string | Plugins.Plugin, enabled?: boolean): Plugins.PluginApi {
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

  animate(
    target: Anim.Keyframes | CAnimation,
    options?: Anim.ControlOptions | (Anim.ControlOptions & Anim.LazyOptions)
  ): Anim.Completing {
    const copiedTarget = recursiveCopy(target, CObject)
    const copiedOptions = recursiveCopy(options)
    const ctx = { target: copiedTarget, options: copiedOptions, promise: this._anim }
    this._plugins.hook(Hooks.animateRegister, ctx).default((ctx) => {
      let activate: (control: AnimControl) => void = () => { }
      const activated = new Promise<AnimControl>((resolve, _reject) => {
        activate = resolve
      })
      const promise = ctx.promise.then(() => this._animate(copiedTarget, copiedOptions, activate))
      ctx.promise = Object.assign(promise, { activated });
    })
    this._anim = ctx.promise
    return this._anim
  }

  _animate(
    target: Anim.Keyframes | CAnimation,
    options: Anim.ControlOptions | (Anim.ControlOptions & Anim.LazyOptions) | undefined,
    activate: (control: AnimControl) => void): Promise<Vizzu> {
    return new Promise((resolve, reject) => {
      const callback = (ok: boolean) => {
        if (ok) {
          resolve(this)
        } else {
          // eslint-disable-next-line prefer-promise-reject-errors
          reject(new CancelError())
          let cancelled = Promise.resolve(this)
          let activated = Promise.reject()
          this._anim = Object.assign(cancelled, { activated })
        }
      }
      if (!this._chart) throw new NotInitializedError()
      this._chart._animate(callback, target, options)
      activate(new AnimControl(this._chart._cChart))
    })
  }

  // keeping this one for backward compatibility
  get animation(): AnimControl {
    if (!this._chart) throw new NotInitializedError()
    return new AnimControl(this._chart._cChart)
  }

  version(): string {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._module.version()
  }

  getCanvasElement(): HTMLCanvasElement {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._canvas
  }

  forceUpdate() {
    if (!this._chart) throw new NotInitializedError()
    this._chart._render.updateFrame(true)
  }

  get data(): Data.Metainfo {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._cData.getMetaInfo()
  }

  get config(): Config.Chart {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._cChart.config.get()
  }

  get style(): Styles.Chart {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._cChart.style.get()
  }

  getComputedStyle(): Styles.Chart {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._cChart.computedStyle.get()
  }

  on(eventName: Event.Type, handler: Event.Handler) {
    if (!this._chart) throw new NotInitializedError()
    this._chart._events.add(eventName, handler)
  }

  off(eventName: Event.Type, handler: Event.Handler) {
    if (!this._chart) throw new NotInitializedError()
    this._chart._events.remove(eventName, handler)
  }

  store(): Snapshot {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._cChart.storeSnapshot()
  }

  getConverter(target: string, from: string, to: string): (point: Geom.Point) => Geom.Point {
    if (!this._chart) throw new NotInitializedError()
    return this._chart.getConverter(target, from, to)
  }

  detach() {
    try {
      this._plugins.destruct()
    } finally {
      this._chart?.destruct()
    }
  }

}

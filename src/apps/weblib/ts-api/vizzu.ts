import {
  Anim,
  Data,
  Config,
  Geom,
  Events,
  Lib,
  Plugins,
  Styles,
  VizzuOptions,
  Features,
  FeatureFunction,
  Vizzu as VizzuInterface
} from './types/vizzu.js'
import { loader } from './module/loader.js'
import { Chart } from './chart.js'
import { CAnimation, Snapshot } from './module/cchart.js'
import { CObject } from './module/cenv.js'
import { AnimControl } from './animcontrol.js'
import { recursiveCopy } from './utils.js'
import { NotInitializedError, CancelError } from './errors.js'
import { Hooks, PluginRegistry } from './plugins.js'
import Presets from './plugins/presets.js'

export default class Vizzu implements VizzuInterface {
  _chart?: Chart
  initializing: Promise<VizzuInterface>
  _anim: Anim.Completing
  _container: HTMLElement
  _plugins: PluginRegistry

  static get presets(): Presets {
    return new Presets()
  }

  static options(options: Lib.Options): void {
    loader.options = options
  }

  static initialize(): Promise<void> {
    return loader.initialize().then(() => {})
  }

  constructor(options: string | HTMLElement | VizzuOptions, initState?: Anim.Keyframes) {
    const opts = this._processOptions(options)

    this._container = opts.container

    this._plugins = new PluginRegistry(this, opts.features)

    this.initializing = loader.initialize().then((module) => {
      this._chart = new Chart(module, this._container, this._plugins)
      this._chart.registerBuilts()
      this._chart.start()
      return this
    })

    this._anim = Object.assign(this.initializing, { activated: Promise.resolve(null) })

    if (initState) {
      this.initializing = this.animate(initState, 0).then(() => this)
    }
  }

  _processOptions(options: unknown): VizzuOptions {
    const opts =
      typeof options !== 'object' || options instanceof HTMLElement
        ? { container: options }
        : options

    if (opts === null || !('container' in opts)) {
      throw new Error('container not specified')
    }

    let container = opts.container

    if (typeof container === 'string') {
      container = document.getElementById(container)
    }

    if (!(container instanceof HTMLElement)) {
      throw new Error(`Cannot find container ${opts.container} to render Vizzu!`)
    }

    return { ...opts, container }
  }

  get feature(): Features {
    const fn: FeatureFunction = this._feature.bind(this)
    return new Proxy(fn, {
      get: (_target, pluginName: string): Plugins.PluginApi => {
        return this._plugins.api(pluginName)
      }
    }) as Features
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
    const ctx = Object.assign(
      { target: copiedTarget, promise: this._anim },
      copiedOptions !== undefined ? { options: copiedOptions } : {}
    )
    this._plugins.hook(Hooks.animateRegister, ctx).default((ctx) => {
      let activate: (control: AnimControl) => void = () => {}
      const activated = new Promise<AnimControl>((resolve) => {
        activate = resolve
      })
      const promise = ctx.promise.then(() => this._animate(copiedTarget, copiedOptions, activate))
      ctx.promise = Object.assign(promise, { activated })
    })
    this._anim = ctx.promise
    return this._anim
  }

  _animate(
    target: Anim.Keyframes | CAnimation,
    options: Anim.ControlOptions | (Anim.ControlOptions & Anim.LazyOptions) | undefined,
    activate: (control: AnimControl) => void
  ): Promise<Vizzu> {
    return new Promise((resolve, reject) => {
      const callback = (ok: boolean): void => {
        if (ok) {
          resolve(this)
        } else {
          // eslint-disable-next-line prefer-promise-reject-errors
          reject(new CancelError())
          const cancelled = Promise.resolve(this)
          const activated = Promise.resolve(null)
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

  forceUpdate(): void {
    if (!this._chart) throw new NotInitializedError()
    this._chart._render.updateFrame(true)
  }

  get data(): Readonly<Data.Metainfo> {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._cData.getMetaInfo()
  }

  get config(): Readonly<Config.Chart> {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._cChart.config.get()
  }

  get style(): Readonly<Styles.Chart> {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._cChart.style.get()
  }

  getComputedStyle(): Styles.Chart {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._cChart.computedStyle.get()
  }

  on<T extends Events.Type>(eventName: T, handler: Events.Handler<Events.EventMap[T]>): void {
    if (!this._chart) throw new NotInitializedError()
    this._chart._events.add(eventName, handler)
  }

  off<T extends Events.Type>(eventName: T, handler: Events.Handler<Events.EventMap[T]>): void {
    if (!this._chart) throw new NotInitializedError()
    this._chart._events.remove(eventName, handler)
  }

  store(): Snapshot {
    if (!this._chart) throw new NotInitializedError()
    return this._chart._cChart.storeSnapshot()
  }

  getConverter(
    target: `plot${string}`,
    from: Geom.CoordinateType,
    to: Geom.CoordinateType
  ): (point: Geom.Point) => Geom.Point {
    if (!this._chart) throw new NotInitializedError()
    return this._chart.getConverter(target, from, to)
  }

  detach(): void {
    try {
      this._plugins.destruct()
    } finally {
      this._chart?.destruct()
    }
  }
}

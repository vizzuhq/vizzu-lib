import { Anim } from './types/anim.js'
import { Data } from './types/data.js'
import { Config } from './types/config.js'
import { Geom } from './types/geom.js'
import { Events } from './types/events.js'
import { Lib } from './types/lib.js'
import { Plugins } from './types/plugins.js'
import { Styles } from './types/styles.js'
import { VizzuOptions, Features, FeatureFunction } from './types/vizzu.js'
import { loader } from './module/loader.js'
import { Chart } from './chart.js'
import { Snapshot } from './module/cchart.js'
import { CAnimation } from './module/canimctrl.js'
import { CObject } from './module/cenv.js'
import { AnimControl } from './animcontrol.js'
import { recursiveCopy } from './utils.js'
import { NotInitializedError, CancelError } from './errors.js'
import { Hooks, PluginRegistry } from './plugins.js'
import Presets from './plugins/presets.js'

/** Class representing a single chart in Vizzu. */
export default class Vizzu {
  /** Promise representing the initialization will resolve when 
    initialization is finished. Any API call will potentially cause 
    an error before this promise is resolved.  */
  initializing: Promise<Vizzu>
  private _chart?: Chart
  private _container: HTMLElement
  private _anim: Anim.Completing
  private _plugins: PluginRegistry

  /** Returns the chart preset collection. */
  static get presets(): Presets {
    return new Presets()
  }

  /** Setter method for Library options. */
  static options(options: Lib.Options): void {
    loader.options = options
  }

  /** Initializes the library, if not called, first constructor call will do that. */
  static initialize(): Promise<void> {
    return loader.initialize().then(() => {})
  }

  /** Creates a new chart and connects it to the div or canvas HTML 
    element specified by its ID or DOM object. The new chart is empty by 
    default, but can be set to an initial state in the second optional 
    parameter. */
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

    this._anim = this.initializing

    if (initState) {
      this.initializing = this.animate(initState, { duration: 0 }).then(() => this)
    }
  }

  private _processOptions(options: unknown): VizzuOptions {
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

  /** If called as a function:
      (name, enabled): it enables/disables built-in features and registered plugins. 
      (plugin, enabled?): registers the given plugin.
      Otherwise gives access to the interfaces of the registered plugins, where
      every plugin acceccible as a property with the plugin name. */
  get feature(): Features {
    const fn: FeatureFunction = this._feature.bind(this)
    return new Proxy(fn, {
      get: (_target, pluginName: string): Plugins.PluginApi => {
        return this._plugins.api(pluginName)
      }
    }) as Features
  }

  private _feature(nameOrInstance: string | Plugins.Plugin, enabled?: boolean): Plugins.PluginApi {
    if (enabled !== undefined && typeof enabled !== 'boolean')
      throw new Error('enabled parameter must be boolean if specified')

    let name
    let enabledInRegister = false
    if (typeof nameOrInstance !== 'string') {
      const instance = nameOrInstance
      name = this._plugins.getRegisteredName(instance)
      if (!name) {
        name = this._plugins.register(instance, enabled ?? true)
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

  /** Initiates the animation either to the new chart state passed as the first 
    argument, or through a sequence of keyframe charts passed as the first
    argument. If there is a currently running animation, all subsequent 
    calls will schedule the corresponding animation after the end of the 
    previous one.
    The new chart state or keyframe can be a full state specifier object with 
    data, config and style, or a single chart config object.
    It accepts also a chart snapshot acquired from a previous state using 
    the store() method of this class or a whole previous animation acquired
    using the store() method of the Anim.Control object, which can be queried
    from the promise returned by the animate() method.
    The optional second parameter specifies the animation control options 
    and also all the other animation options in case of only a single chart
    state passed as the first argument. 
    This second option can be a scalar value, setting the overall 
    animation duration. Passing explicit null as second parameter will
    result in no animation.
    The method returns a promise, which will resolve when the animation is
    finished. Since there can be multiple animations in the queue, the result
    promise provides a nested promise member {@link Anim.Completing.activated|activated}, 
    which resolves when the requested animation gets active.  */
  animate(target: Anim.Keyframes | CAnimation, options?: Anim.ControlOptions): Anim.Completing {
    const copiedTarget = recursiveCopy(target, CObject)
    const copiedOptions = recursiveCopy(options)
    const ctx = Object.assign(
      { target: copiedTarget, promise: this._anim },
      copiedOptions !== undefined ? { options: copiedOptions } : {}
    )
    this._plugins.hook(Hooks.registerAnimation, ctx).default((ctx) => {
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

  private _animate(
    target: Anim.Keyframes | CAnimation,
    options: Anim.ControlOptions | undefined,
    activate: (control: AnimControl) => void
  ): Promise<Vizzu> {
    return new Promise(async (resolve, reject) => {
      const callback = (ok: boolean): void => {
        if (ok) {
          resolve(this)
        } else {
          // eslint-disable-next-line prefer-promise-reject-errors
          reject(new CancelError())
          this._anim = Promise.resolve(this)
        }
      }
      if (!this._chart) throw new NotInitializedError()
      await this._chart.prepareAnimation(target, options)
      this._chart.runAnimation(callback)
      activate(new AnimControl(this._chart.getCAnimControl()))
    })
  }

  /** Returns controls for the ongoing animation, if any.
    @deprecated since version 0.4.0  */
  get animation(): Anim.Control {
    if (!this._chart) throw new NotInitializedError()
    return new AnimControl(this._chart.getCAnimControl())
  }

  /** Returns the version number of the library. */
  version(): string {
    if (!this._chart) throw new NotInitializedError()
    return this._chart.version()
  }

  /** Returns the underlying canvas element. */
  getCanvasElement(): HTMLCanvasElement {
    if (!this._chart) throw new NotInitializedError()
    return this._chart.getCanvasElement()
  }

  /** Re-renders the chart. */
  forceUpdate(): void {
    if (!this._chart) throw new NotInitializedError()
    this._chart.forceUpdate()
  }

  /** Property for read-only access to data metainfo object. */
  get data(): Readonly<Data.Metainfo> {
    if (!this._chart) throw new NotInitializedError()
    return this._chart.data
  }

  /** Property for read-only access to chart parameter object. */
  get config(): Readonly<Config.Chart> {
    if (!this._chart) throw new NotInitializedError()
    return this._chart.config
  }

  /** Property for read-only access to style object without default values. */
  get style(): Readonly<Styles.Chart> {
    if (!this._chart) throw new NotInitializedError()
    return this._chart.style
  }

  /** Property for read-only access to the style object after setting defaults. */
  getComputedStyle(): Readonly<Styles.Chart> {
    if (!this._chart) throw new NotInitializedError()
    return this._chart.getComputedStyle()
  }

  /** Installs the provided event handler to the event specified by name. */
  on<T extends Events.Type>(eventName: T, handler: Events.Handler<Events.EventMap[T]>): void {
    if (!this._chart) throw new NotInitializedError()
    this._chart.on(eventName, handler)
  }

  /** Uninstalls the provided event handler from the event specified by name. */
  off<T extends Events.Type>(eventName: T, handler: Events.Handler<Events.EventMap[T]>): void {
    if (!this._chart) throw new NotInitializedError()
    this._chart.off(eventName, handler)
  }

  /** Returns a reference to the actual chart state for further reuse. 
    This reference includes the chart config, style parameters and the
    data filter but does not include the actual data and the animation options. */
  store(): Snapshot {
    if (!this._chart) throw new NotInitializedError()
    return this._chart.store()
  }

  /** Returns a converter function. */
  getConverter(
    target: `plot${string}`,
    from: Geom.CoordinateType,
    to: Geom.CoordinateType
  ): Geom.Converter {
    if (!this._chart) throw new NotInitializedError()
    return this._chart.getConverter(target, from, to)
  }

  /** Removes the reference of the chart from every place it attached itself,
    this method must be called in order to get the chart properly garbage 
    collected.  */
  detach(): void {
    try {
      this._plugins.destruct()
    } finally {
      this._chart?.destruct()
    }
  }
}

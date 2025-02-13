import * as Anim from './types/anim.js'
import * as Data from './types/data.js'
import * as Config from './types/config.js'
import * as Styles from './types/styles.js'
import { EventType, EventMap, EventHandler } from './events.js'
import { loader, LoaderOptions } from './module/loader.js'
import { Chart } from './chart.js'
import { Snapshot } from './module/cchart.js'
import { CObject } from './module/cenv.js'
import { AnimControl } from './animcontrol.js'
import { AnimCompleting } from './animcompleting.js'
import { recursiveCopy } from './utils.js'
import { Mirrored } from './tsutils.js'
import { NotInitializedError, CancelError } from './errors.js'
import { Plugin, PluginApi, PluginRegistry, Hooks } from './plugins.js'
import Presets from './plugins/presets.js'
import { LazyCanvasOptions, HtmlCanvasApi } from './plugins/htmlcanvas.js'
import { CoordSystemApi } from './plugins/coordsys.js'

/** Options for the library. */
export type LibOptions = LoaderOptions

/** List of base and additional features:
    - logging: enables logging of the library to the console 
      (switched off by default).
    - rendering: enables rendering of the library to the canvas
      (enabled by default). 
    - tooltip: tooltips on the chart appearing on markers on mouse over. 
      Since the tooltip uses the animation interface, calling animate() while
      the tooltip is enabled can cause unwanted behaviour.
    - cssProperties: enables setting the styles through --vizzu-... css properties.
    - shorthands: enables shorthand properties for chart config and style.
    - pivotData: enables setting the data in pivot table or data cube format.
    - pointerEvents: enables pointer events on the chart.
  */
export type Feature =
	| 'logging'
	| 'tooltip'
	| 'rendering'
	| 'cssProperties'
	| 'coordSystem'
	| 'htmlCanvas'
	| 'shorthands'
	| 'pivotData'
	| 'pointerEvents'

export interface FeatureOptions {
	features?: Plugin[]
}

export interface OtherSource {
	series: Data.SeriesMetaInfo[]
	aggregate: (filter1: string | null, filter2: string | null, groupBy: Data.SeriesList, series: Data.SeriesList) => Data.Set
}

export interface OtherSourceOptions {
	otherSource?: OtherSource
}

export type VizzuOptions = FeatureOptions & OtherSourceOptions & LazyCanvasOptions

export type FeatureFunction = (feature: Feature | Plugin, enabled?: boolean) => PluginApi
export interface Features extends Record<string, PluginApi>, FeatureFunction {
	coordSystem: CoordSystemApi
	htmlCanvas: HtmlCanvasApi
}

/** Class representing a single chart in Vizzu. */
export default class Vizzu {
	/** Promise representing the initialization will resolve when 
    initialization is finished. Any API call will potentially cause 
    an error before this promise is resolved.  */
	initializing: Promise<Vizzu>
	private _chart?: Chart
	private _anim: AnimCompleting
	private _plugins: PluginRegistry

	/** Returns the chart preset collection. */
	static get presets(): Presets {
		return new Presets()
	}

	/** Setter method for Library options. */
	static options(options: LibOptions): void {
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
	constructor(options: VizzuOptions, initState?: Anim.Target) {
		this._plugins = new PluginRegistry(this, this._getFeatureList(options))

		this.initializing = loader.initialize().then((module) => {
			this._chart = new Chart(module, options, this._plugins)
			this._chart.registerBuiltins()
			this._chart.start()
			return this
		})

		this._anim = this.initializing

		if (initState) {
			this.initializing = this.animate([
				{
					target: initState,
					options: { duration: 0 }
				}
			]).then(() => this)
		}
	}

	private _getFeatureList(options: VizzuOptions): Plugin[] {
		if (typeof options === 'object' && options !== null && 'features' in options) {
			return options.features
		}
		return [] as Plugin[]
	}

	/** If called as a function:
      (name, enabled): it enables/disables built-in features and registered plugins. 
      (plugin, enabled?): registers the given plugin.
      Otherwise gives access to the interfaces of the registered plugins, where
      every plugin acceccible as a property with the plugin name. */
	get feature(): Features {
		const fn: FeatureFunction = this._feature.bind(this)
		return new Proxy(fn, {
			get: (_target, pluginName: string): PluginApi => {
				return this._plugins.api(pluginName)
			}
		}) as Features
	}

	private _feature(nameOrInstance: string | Plugin, enabled?: boolean): PluginApi {
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
    using the store() method of the AnimControl object, which can be queried
    from the promise returned by the animate() method.
    The optional second parameter specifies the animation control options 
    and also all the other animation options in case of only a single chart
    state passed as the first argument. 
    This second option can be a scalar value, setting the overall 
    animation duration. Passing explicit null as second parameter will
    result in no animation.
    The method returns a promise, which will resolve when the animation is
    finished. Since there can be multiple animations in the queue, the result
    promise provides a nested promise member `activated`, 
    which resolves when the requested animation gets active.  */
	animate(target: Anim.AnimTarget, options?: Anim.ControlOptions): AnimCompleting {
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
			const promise = ctx.promise.then(() =>
				this._animate(copiedTarget, copiedOptions, activate)
			)
			ctx.promise = Object.assign(promise, { activated })
		})
		this._anim = ctx.promise
		return this._anim
	}

	private _animate(
		target: Anim.AnimTarget,
		options: Anim.ControlOptions | undefined,
		activate: (control: AnimControl) => void
	): Promise<Vizzu> {
		if (!this._chart) throw new NotInitializedError()
		return this._chart.prepareAnimation(target, options).then(() => {
			return this._runAnimation(activate)
		})
	}

	private _runAnimation(activate: (control: AnimControl) => void): Promise<Vizzu> {
		return new Promise((resolve, reject) => {
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
			this._chart.runAnimation(callback)
			activate(this._chart.getAnimControl())
		})
	}

	/** Returns controls for the ongoing animation, if any.
    @deprecated since version 0.4.0  */
	get animation(): AnimControl {
		if (!this._chart) throw new NotInitializedError()
		return this._chart.getAnimControl()
	}

	/** Returns the version number of the library. */
	version(): string {
		if (!this._chart) throw new NotInitializedError()
		return this._chart.version()
	}

	/** Property for read-only access to data metainfo object. */
	get data(): Mirrored<Data.Metainfo> {
		if (!this._chart) throw new NotInitializedError()
		return this._chart.data
	}

	/** Property for read-only access to chart parameter object. */
	get config(): Mirrored<Config.Chart> {
		if (!this._chart) throw new NotInitializedError()
		return this._chart.config
	}

	/** Property for read-only access to style object without default values. */
	get style(): Mirrored<Styles.Chart> {
		if (!this._chart) throw new NotInitializedError()
		return this._chart.style
	}

	/** Property for read-only access to the style object after setting defaults. */
	getComputedStyle(): Mirrored<Styles.Chart> {
		if (!this._chart) throw new NotInitializedError()
		return this._chart.getComputedStyle()
	}

	/** Installs the provided event handler to the event specified by name. */
	on<T extends EventType>(eventName: T, handler: EventHandler<EventMap[T]>): void {
		if (!this._chart) throw new NotInitializedError()
		this._chart.on(eventName, handler)
	}

	/** Uninstalls the provided event handler from the event specified by name. */
	off<T extends EventType>(eventName: T, handler: EventHandler<EventMap[T]>): void {
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

	/** Removes the reference of the chart from every place it attached itself,
    this method must be called in order to get the chart properly garbage 
    collected.  */
	detach(): void {
		try {
			this._plugins.destruct()
			this._chart?.detach()
		} catch (e) {
			console.error(`Error during plugin destruct: ${e}`)
		}
	}
}

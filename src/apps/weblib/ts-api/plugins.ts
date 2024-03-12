import Vizzu from './vizzu.js'
import * as Anim from './types/anim.js'
import { Events, EventType, EventHandler, EventMap } from './events.js'
import { AnimCompleting } from './animcompleting.js'
import { Point } from './geom.js'
import { CRenderer } from './module/crenderer.js'
import { Canvas } from './module/canvas.js'

/** Available hooks for plugins in Vizzu. */
export enum Hooks {
	/** Called once on startup for start the rendering loop. */
	start = 'start',
	/** Called on rendering. */
	render = 'render',
	/** Called when the animate() parameters gets set in the library to prepare 
      the animation. */
	prepareAnimation = 'prepareAnimation',
	/** Called when the animate() method called, and the lib shedules the call 
      to the animation queue. */
	registerAnimation = 'registerAnimation',
	/** Called when all animate() parameter set and animation can be started. */
	runAnimation = 'runAnimation'
}

/** Plugin metainfo. */
export interface PluginMeta {
	/** Name of the plugin. If not set, the class name will be used 
          with a lowercase first char. */
	name?: string
	/** Version of the plugin. Not used for now. */
	version?: string
	/** List of plugins this plugin depends on. 
          Dependent plugins should be registered beforehand. */
	depends?: string[]
}

export interface StartContext {
	update: (force: boolean) => void
}

export interface RenderContext {
	renderer: (CRenderer & Canvas) | null
	timeInMSecs: number | null
	enable: boolean
	force: boolean
	size: Point
}

export interface PrepareAnimationContext {
	target: Anim.AnimTarget
	options?: Anim.ControlOptions
}

export interface RegisterAnimationContext {
	target: Anim.AnimTarget
	options?: Anim.ControlOptions
	promise: AnimCompleting
}

export interface RunAnimationContext {
	callback: (ok: boolean) => void
}

export interface HookContexts {
	[Hooks.start]: StartContext
	[Hooks.render]: RenderContext
	[Hooks.prepareAnimation]: PrepareAnimationContext
	[Hooks.registerAnimation]: RegisterAnimationContext
	[Hooks.runAnimation]: RunAnimationContext
}

type Next = () => void

/** Plugin hook implementation. Plugin hooks gets called by Vizzu in 
        priority order. Each hook should call the next hook in the chain,
        or the default implementation won't run. */
type PluginHook<T> = {
	(ctx: T, next: Next): void
	priority?: number
}

/** Set of plugin hook implementations. */
export type PluginHooks<T extends Hooks = Hooks> = {
	[key in T]?: PluginHook<HookContexts[key]>
}

export type PluginListeners = {
	[event in EventType]?: EventHandler<EventMap[event]>
}

export interface PluginApi {
	[apiMethod: string]: unknown
}

/** Vizzu plugin interface. */
export interface Plugin {
	/** Metainfo about the plugin. */
	meta?: PluginMeta
	/** Hooks the plugin implemenst. They work only if the plugin is enabled. */
	hooks?: PluginHooks
	/** Event listeners the plugin implements. They work only if the plugin is enabled. */
	listeners?: PluginListeners
	/** Any parameter or method the Plugin exposes for the user. */
	api?: PluginApi
	/** Register called when the plugin added to vizzu, receiving the Vizzu instance. */
	register?: (ctx: Vizzu) => void
	/** Unregister is called when detach() called on the Vizzu instance. */
	unregister?: (ctx: Vizzu) => void
	/** Method called by Vizzu indicating for the plugin that it got switched on/off.
          If switched off, its event listeners gets removed and its hooks won't be called. */
	enable?: (enabled: boolean) => void
}

interface PluginRecord {
	name: string
	instance: Plugin
	enabled: boolean
	registeredListeners?: PluginListeners
}

interface HookExecutor<T extends Hooks> {
	default: (last?: (ctx: HookContexts[T]) => void) => void
}

export class PluginRegistry {
	private _parent: Vizzu | null
	private _plugins: PluginRecord[]
	private _events?: Events

	constructor(parent: Vizzu, plugins?: Plugin[]) {
		this._parent = parent
		this._plugins = []
		if (plugins) {
			if (!Array.isArray(plugins)) {
				throw new Error('Plugins must be an array')
			}
			plugins.forEach((plugin) => this.register(plugin, true))
		}
	}

	init(events: Events): void {
		this._events = events
		this._plugins.forEach((plugin) => {
			if (plugin.enabled) {
				this._enableEvents(plugin)
			}
		})
	}

	enable(name: string, enabled: boolean): void {
		const plugin = this._getByName(name)
		this._setEnabled(plugin, enabled)
	}

	getRegisteredName(instance: Plugin): string | undefined {
		const plugin = this._plugins.find((plugin) => plugin.instance === instance)
		return plugin?.name
	}

	register(instance: Plugin, enabled = true): string {
		this._validate(instance)
		const name = this._discoverName(instance)!
		if (instance.register && this._parent) {
			instance.register(this._parent)
		}
		const plugin = { name, instance, enabled: !enabled }
		this._plugins.push(plugin)
		this._setEnabled(plugin, enabled)
		return name
	}

	destruct(): void {
		const names = this._plugins.map((plugin) => plugin.name)
		try {
			names.forEach((name) => this.unregister(name))
		} finally {
			this._plugins = []
			this._parent = null
		}
	}

	unregister(name: string): void {
		const index = this._plugins.findIndex((plugin) => plugin.name === name)
		if (index !== -1) {
			const plugin = this._plugins[index]
			this._plugins.splice(index, 1)
			if (plugin?.instance.unregister && this._parent) {
				plugin.instance.unregister(this._parent)
			}
		}
	}

	api(name: string): PluginApi {
		const plugin = this._getByName(name)
		return plugin.instance.api || {}
	}

	hook<T extends Hooks>(type: T, ctx: HookContexts[T]): HookExecutor<T> {
		return {
			default: (last: (ctx: HookContexts[T]) => void = (): void => {}): void => {
				this._exec(ctx, type, last)
			}
		}
	}

	private _setEnabled(plugin: PluginRecord, enabled: boolean): void {
		if (plugin.enabled !== enabled) {
			plugin.enabled = enabled
			if (plugin.instance.enable) {
				plugin.instance.enable(enabled)
			}
			if (plugin.enabled) {
				this._enableEvents(plugin)
			} else {
				this._disableEvents(plugin)
			}
		}
	}

	private _enableEvents(plugin: PluginRecord): void {
		if (plugin.instance.listeners) {
			plugin.registeredListeners = plugin.instance.listeners
			this._events?.addMany(plugin.registeredListeners)
		}
	}

	private _disableEvents(plugin: PluginRecord): void {
		if (plugin.registeredListeners) {
			this._events?.removeMany(plugin.registeredListeners)
		}
		delete plugin.registeredListeners
	}

	private _validate(instance: Plugin): void {
		this._validateName(instance)
		this._validateDepends(instance)
	}

	private _validateName(instance: Plugin): void {
		const name = this._discoverName(instance)
		if (typeof name === 'string' && name.length > 0) {
			return
		}
		throw new Error('Plugin must have a non empty name string')
	}

	private _discoverName(instance: Plugin): string | undefined {
		return (
			instance.meta?.name ??
			(instance.constructor?.name ? this._firstToLower(instance.constructor.name) : undefined)
		)
	}

	private _firstToLower(s: string): string {
		if (typeof s === 'string' && s[0]) {
			return s[0].toLowerCase() + s.slice(1)
		}
		return ''
	}

	private _validateDepends(instance: Plugin): void {
		if (instance.meta?.depends) {
			if (Array.isArray(instance.meta.depends)) {
				for (const depend of instance.meta.depends) {
					this._getByName(depend)
				}
				return
			}
			throw new Error('Plugin depends must be an array')
		}
	}

	private _getByName(name: string): PluginRecord {
		const plugin = this._plugins.find((plugin) => plugin.name === name)
		if (plugin) return plugin
		throw new Error(`Plugin ${name} is not registered`)
	}

	private _exec<T extends Hooks>(
		ctx: HookContexts[T],
		type: T,
		last: PluginHook<HookContexts[T]>
	): void {
		const hooks = [...this._getHooks(type), last]
		this._executeHooks(hooks, ctx)
	}

	private _getHooks<T extends Hooks>(type: T): PluginHook<HookContexts[T]>[] {
		return this._plugins
			.filter((plugin) => plugin.enabled && plugin.instance.hooks?.[type])
			.map((plugin: PluginRecord) => plugin.instance.hooks![type]!)
			.sort((a, b) => (b.priority || 0) - (a.priority || 0))
	}

	private _executeHooks<T>(hooks: PluginHook<T>[], ctx: T): void {
		let next = (): void => {}
		const iterator = hooks
			.map((fn) => {
				return fn ? (): void => fn(ctx, next) : (): void => next()
			})
			[Symbol.iterator]()
		next = (): void => iterator.next().value()
		next()
	}
}

import { Plugins as P } from './types/vizzu.js'

import Vizzu from './vizzu.js'

import { Events } from './events.js'

export enum Hooks {
  setAnimParams = 'setAnimParams',
  animateRegister = 'animateRegister'
}

interface PluginRecord {
  name: string
  instance: P.Plugin
  enabled: boolean
  registeredListeners?: P.PluginListeners
}

interface HookExecutor<T extends Hooks> {
  default: (last?: (ctx: P.HookContexts[T]) => void) => void
}

export class PluginRegistry {
  private _parent: Vizzu | null
  private _plugins: PluginRecord[]
  private _events?: Events

  constructor(parent: Vizzu, plugins?: P.Plugin[]) {
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

  getRegisteredName(instance: P.Plugin): string | undefined {
    const plugin = this._plugins.find((plugin) => plugin.instance === instance)
    return plugin?.name
  }

  register(instance: P.Plugin, enabled = true): string {
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

  api(name: string): P.PluginApi {
    const plugin = this._getByName(name)
    return plugin.instance.api || {}
  }

  hook<T extends Hooks>(type: T, ctx: P.HookContexts[T]): HookExecutor<T> {
    return {
      default: (last: (ctx: P.HookContexts[T]) => void = (): void => {}): void => {
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

  private _validate(instance: P.Plugin): void {
    this._validateName(instance)
    this._validateDepends(instance)
  }

  private _validateName(instance: P.Plugin): void {
    const name = this._discoverName(instance)
    if (typeof name === 'string' && name.length > 0) {
      return
    }
    throw new Error('Plugin must have a non empty name string')
  }

  private _discoverName(instance: P.Plugin): string | undefined {
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

  private _validateDepends(instance: P.Plugin): void {
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
    ctx: P.HookContexts[T],
    type: T,
    last: P.PluginHook<P.HookContexts[T]>
  ): void {
    const hooks = [...this._getHooks(type), last]
    this._executeHooks(hooks, ctx)
  }

  private _getHooks<T extends Hooks>(type: T): P.PluginHook<P.HookContexts[T]>[] {
    return this._plugins
      .filter((plugin) => plugin.enabled && plugin.instance.hooks?.[type])
      .map((plugin: PluginRecord) => plugin.instance.hooks![type]!)
      .sort((a, b) => (b.priority || 0) - (a.priority || 0))
  }

  private _executeHooks<T>(hooks: P.PluginHook<T>[], ctx: T): void {
    const iterator = hooks
      .map((fn) => {
        return fn ? (): void => fn(ctx, next) : (): void => next()
      })
      [Symbol.iterator]()
    const next = (): void => iterator.next().value()

    next()
  }
}

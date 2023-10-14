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

export class PluginRegistry {
  _parent: Vizzu | null
  _plugins: PluginRecord[]
  _events?: Events

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

  init(events: Events) {
    this._events = events
    this._plugins.forEach((plugin) => {
      if (plugin.enabled) {
        this._enableEvents(plugin)
      }
    })
  }

  enable(name: string, enabled: boolean) {
    const plugin = this._getByName(name)
    this._setEnabled(plugin, enabled)
  }

  getRegisteredName(instance: P.Plugin) {
    const plugin = this._plugins.find((plugin) => plugin.instance === instance)
    return plugin?.name
  }

  register(instance: P.Plugin, enabled = true): string {
    this._validate(instance)
    const name = this._discoverName(instance)
    if (instance.register && this._parent) {
      instance.register(this._parent)
    }
    const plugin = { name, instance, enabled: !enabled }
    this._plugins.push(plugin)
    this._setEnabled(plugin, enabled)
    return name
  }

  destruct() {
    const names = this._plugins.map((plugin) => plugin.name)
    try {
      names.forEach((name) => this.unregister(name))
    } finally {
      this._plugins = []
      this._parent = null
    }
  }

  unregister(name: string) {
    const index = this._plugins.findIndex((plugin) => plugin.name === name)
    if (index !== -1) {
      const plugin = this._plugins[index]
      this._plugins.splice(index, 1)
      if (plugin?.instance.unregister && this._parent) {
        plugin.instance.unregister(this._parent)
      }
    }
  }

  api(name: string) {
    const plugin = this._getByName(name)
    return plugin.instance.api || {}
  }

  hook<T extends Hooks>(type: T, ctx: P.HookContexts[T]) {
    return {
      default: (last: (ctx: P.HookContexts[T]) => void = () => {}) => {
        this._exec(ctx, type, last)
      }
    }
  }

  _setEnabled(plugin: PluginRecord, enabled: boolean) {
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

  _enableEvents(plugin: PluginRecord) {
    if (plugin.instance.listeners) {
      plugin.registeredListeners = plugin.instance.listeners
      this._events?.addMany(plugin.registeredListeners)
    }
  }

  _disableEvents(plugin: PluginRecord) {
    if (plugin.registeredListeners) {
      this._events?.removeMany(plugin.registeredListeners)
    }
    delete plugin.registeredListeners
  }

  _validate(instance: P.Plugin) {
    this._validateName(instance)
    this._validateDepends(instance)
  }

  _validateName(instance: P.Plugin) {
    const name = this._discoverName(instance)
    if (typeof name === 'string' && name.length > 0) {
      return
    }
    throw new Error('Plugin must have a non empty name string')
  }

  _discoverName(instance: P.Plugin) {
    return (
      instance.meta?.name ??
      (instance.constructor?.name ? this._firstToLower(instance.constructor.name) : null)
    )
  }

  _firstToLower(s: string): string {
    if (typeof s === 'string' && s[0]) {
      return s[0].toLowerCase() + s.slice(1)
    }
    return ''
  }

  _validateDepends(instance: P.Plugin) {
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

  _getByName(name: string): PluginRecord {
    const plugin = this._plugins.find((plugin) => plugin.name === name)
    if (plugin) return plugin
    throw new Error(`Plugin ${name} is not registered`)
  }

  _exec<T extends Hooks>(ctx: P.HookContexts[T], type: T, last: P.PluginHook<P.HookContexts[T]>) {
    const hooks = [...this._getHooks(type), last]
    this._executeHooks(hooks, ctx)
  }

  _getHooks<T extends Hooks>(type: T): P.PluginHook<P.HookContexts[T]>[] {
    return this._plugins
      .filter((plugin) => plugin.enabled && plugin.instance.hooks?.[type])
      .map((plugin: PluginRecord) => plugin.instance.hooks![type]!)
      .sort((a, b) => (b.priority || 0) - (a.priority || 0))
  }

  _executeHooks<T>(hooks: P.PluginHook<T>[], ctx: T) {
    const iterator = hooks
      .map((fn) => {
        return fn ? () => fn(ctx, next) : () => next()
      })
      [Symbol.iterator]()
    const next = () => iterator.next().value()

    return next()
  }
}

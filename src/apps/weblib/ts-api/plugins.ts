/// <reference types="./types/vizzu.d.ts" />

import { Events } from './events'

type Hook<T> = Plugins.PluginHook<T>

interface PluginRecord {
  name: string
  instance: Plugins.Plugin
  enabled: boolean
  registeredListeners?: Plugins.PluginListeners
}

export class Plugins {
  _parent: Vizzu | null
  _plugins: PluginRecord[]
  _events?: Events

  constructor(parent: Vizzu, plugins?: Plugins.Plugin[]) {
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

  getRegisteredName(instance: Plugins.Plugin) {
    const plugin = this._plugins.find((plugin) => plugin.instance === instance)
    return plugin?.name
  }

  register(instance: Plugins.Plugin, enabled = true): string {
    this._validate(instance)
    const name = this._discoverName(instance)
    if (instance.register && this._parent) {
      instance.register(this._parent)
    }
    const plugin = { name, instance, enabled: !enabled } as PluginRecord
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
      if (plugin.instance.unregister && this._parent) {
        plugin.instance.unregister(this._parent)
      }
    }
  }

  api(name: string) {
    const plugin = this._getByName(name)
    return plugin.instance.api || {}
  }

  hook<T>(type: string, ctx: T) {
    return {
      default: (last = (_ctx: T) => { }) => {
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

  _validate(instance: Plugins.Plugin) {
    this._validateName(instance)
    this._validateDepends(instance)
  }

  _validateName(instance: Plugins.Plugin) {
    const name = this._discoverName(instance)
    if (typeof name === 'string' && name.length > 0) {
      return
    }
    throw new Error('Plugin must have a non empty name string')
  }

  _discoverName(instance: Plugins.Plugin) {
    return (
      instance.meta?.name ??
      (instance.constructor?.name ? this._firstToLower(instance.constructor.name) : null)
    )
  }

  _firstToLower(s: string) {
    return s[0].toLowerCase() + s.slice(1)
  }

  _validateDepends(instance: Plugins.Plugin) {
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

  _exec<T>(ctx: T, type: string, last: Hook<T>) {
    const hooks = [...this._getHooks(type), last]
    this._executeHooks(hooks, ctx)
  }

  _getHooks<T>(type: string): Hook<T>[] {
    return this._plugins
      .filter((plugin) => plugin.enabled)
      .map((plugin: PluginRecord) => (plugin.instance.hooks as any)?.[type] as Hook<T>)
      .sort((a, b) => {
        return (b.priority || 0) - (a.priority || 0)
      })
  }

  _executeHooks<T>(hooks: Hook<T>[], ctx: T) {
    const iterator = hooks
      .map((fn) => {
        return fn ? () => fn(ctx, next) : () => next()
      })
    [Symbol.iterator]()
    const next = () => iterator.next().value()

    return next()
  }
}

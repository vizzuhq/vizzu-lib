export default class Plugins {
  constructor(parent, plugins) {
    this._parent = parent
    this._plugins = []
    if (plugins) {
      if (!Array.isArray(plugins)) throw new Error('Plugins must be an array')
      plugins.forEach((plugin) => this.register(plugin, true))
    }
  }

  getName(instance) {
    const plugin = this._plugins.find((plugin) => plugin.instance === instance)
    return plugin?.name
  }

  register(instance, enabled = true) {
    this._validate(instance)
    const name = instance.meta.name
    if (instance.register) {
      try {
        instance.register(this._parent)
      } catch (e) {
        console.error(e)
      }
    }
    const plugin = { name, instance, enabled: undefined }
    this._plugins.push(plugin)
    this._setEnabled(plugin, enabled)
    return name
  }

  enabled(name, enabled) {
    const plugin = this._getByName(name)
    this._setEnabled(plugin, enabled)
  }

  destruct() {
    const names = this._plugins.map((plugin) => plugin.name)
    names.forEach((name) => this.unregister(name))
    this._plugins = []
    this._parent = null
  }

  unregister(name) {
    const index = this._plugins.findIndex((plugin) => plugin.name === name)
    if (index !== -1) {
      const plugin = this._plugins[index]
      if (plugin.instance.unregister) {
        try {
          plugin.instance.unregister(this._parent)
        } catch (e) {
          console.error(e)
        }
      }
      this._plugins.splice(index, 1)
    }
  }

  api(name) {
    const plugin = this._getByName(name)
    return plugin.instance.api || {}
  }

  hook(type, ctx) {
    return {
      default: (last = () => {}) => {
        this._exec(ctx, type, last)
      }
    }
  }

  _setEnabled(plugin, enabled) {
    if (plugin.enabled !== enabled) {
      plugin.enabled = enabled
      if (plugin.instance.enabled) {
        plugin.instance.enabled(enabled)
      }
      if (plugin.instance.listeners) {
        if (plugin.enabled) {
          plugin.listeners = plugin.instance.listeners
          this._parent.events.addMany(plugin.listeners)
        } else {
          if (plugin.listeners) {
            this._parent.events.removeMany(plugin.listeners)
          }
          plugin.listeners = null
        }
      }
    }
  }

  _validate(instance) {
    this._validateName(instance)
    this._validateDepends(instance)
  }

  _validateName(instance) {
    if (instance.meta?.name) {
      if (typeof instance.meta.name === 'string' && instance.meta.name.length > 0) {
        return
      }
    }
    throw new Error('Plugin must have a non empty name string')
  }

  _validateDepends(instance) {
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

  _getByName(name) {
    const plugin = this._plugins.find((plugin) => plugin.name === name)
    if (plugin) return plugin
    throw new Error(`Plugin ${name} is not registered`)
  }

  _exec(ctx, type, last) {
    const hooks = [...this._getHooks(type), last]
    this._executeHooks(hooks, ctx)
  }

  _getHooks(type) {
    return this._plugins
      .filter((plugin) => plugin.enabled)
      .map((plugin) => plugin.instance.hooks?.[type]?.bind(this._parent))
      .sort((a, b) => {
        return (a.priority || 0) - (b.priority || 0)
      })
  }

  _executeHooks(hooks, ctx) {
    const iterator = hooks
      .map((fn) => {
        return fn ? () => fn(ctx, next) : () => next()
      })
      [Symbol.iterator]()
    const next = () => iterator.next().value()

    return next()
  }
}

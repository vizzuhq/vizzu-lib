/** Available hooks for plugins in Vizzu. */
export var Hooks
;(function (Hooks) {
  /** Called when the animate() parameters gets set in the library to prepare
        the animation. */
  Hooks['prepareAnimation'] = 'prepareAnimation'
  /** Called when the animate() method called, and the lib shedules the call
        to the animation queue. */
  Hooks['registerAnimation'] = 'registerAnimation'
  /** Called when all animate() parameter set and animation can be started. */
  Hooks['runAnimation'] = 'runAnimation'
})(Hooks || (Hooks = {}))
export class PluginRegistry {
  constructor(parent, plugins) {
    this._parent = parent
    this._plugins = []
    if (plugins) {
      if (!Array.isArray(plugins)) {
        throw new Error('Plugins must be an array')
      }
      plugins.forEach((plugin) => this.register(plugin, true))
    }
  }
  init(events) {
    this._events = events
    this._plugins.forEach((plugin) => {
      if (plugin.enabled) {
        this._enableEvents(plugin)
      }
    })
  }
  enable(name, enabled) {
    const plugin = this._getByName(name)
    this._setEnabled(plugin, enabled)
  }
  getRegisteredName(instance) {
    const plugin = this._plugins.find((plugin) => plugin.instance === instance)
    return plugin?.name
  }
  register(instance, enabled = true) {
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
  unregister(name) {
    const index = this._plugins.findIndex((plugin) => plugin.name === name)
    if (index !== -1) {
      const plugin = this._plugins[index]
      this._plugins.splice(index, 1)
      if (plugin?.instance.unregister && this._parent) {
        plugin.instance.unregister(this._parent)
      }
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
  _enableEvents(plugin) {
    if (plugin.instance.listeners) {
      plugin.registeredListeners = plugin.instance.listeners
      this._events?.addMany(plugin.registeredListeners)
    }
  }
  _disableEvents(plugin) {
    if (plugin.registeredListeners) {
      this._events?.removeMany(plugin.registeredListeners)
    }
    delete plugin.registeredListeners
  }
  _validate(instance) {
    this._validateName(instance)
    this._validateDepends(instance)
  }
  _validateName(instance) {
    const name = this._discoverName(instance)
    if (typeof name === 'string' && name.length > 0) {
      return
    }
    throw new Error('Plugin must have a non empty name string')
  }
  _discoverName(instance) {
    return (
      instance.meta?.name ??
      (instance.constructor?.name ? this._firstToLower(instance.constructor.name) : undefined)
    )
  }
  _firstToLower(s) {
    if (typeof s === 'string' && s[0]) {
      return s[0].toLowerCase() + s.slice(1)
    }
    return ''
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
      .filter((plugin) => plugin.enabled && plugin.instance.hooks?.[type])
      .map((plugin) => plugin.instance.hooks[type])
      .sort((a, b) => (b.priority || 0) - (a.priority || 0))
  }
  _executeHooks(hooks, ctx) {
    let next = () => {}
    const iterator = hooks
      .map((fn) => {
        return fn ? () => fn(ctx, next) : () => next()
      })
      [Symbol.iterator]()
    next = () => iterator.next().value()
    next()
  }
}

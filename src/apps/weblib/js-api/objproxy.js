export default class ObjectProxy {
  constructor(setter) {
    this._setter = setter
  }

  setupProperties(object, path = null) {
    for (const propName in object)
      if (this._isObject(object[propName])) {
        this.setupProperties(object[propName], ObjectProxy._path(path, propName))
        this._setupProperty(object, path, propName, true)
      } else this._setupProperty(object, path, propName, false)
  }

  _setupProperty(object, path, propName, isObject) {
    const shadowPropName = '_' + propName
    const propPath = ObjectProxy._path(path, propName)
    object[shadowPropName] = object[propName]
    Object.defineProperty(object, shadowPropName, {
      enumerable: false,
      writable: true
    })
    const setter = isObject
      ? this._objectSetter(object, shadowPropName, propPath)
      : this._valueSetter(object, shadowPropName, propPath)
    Object.defineProperty(object, propName, {
      set: setter,
      get: function () {
        return this[shadowPropName]
      },
      enumerable: true
    })
  }

  _valueSetter(object, shadowPropName, propPath) {
    const setter = this._setter
    return (value) => {
      object[shadowPropName] = value
      setter(propPath, value)
    }
  }

  _objectSetter(object, shadowPropName, propPath) {
    const setter = this._setter
    return (value) => {
      ObjectProxy._setObject(value, setter, propPath)
    }
  }

  static _setObject(object, setter, path = null) {
    if (object) {
      Object.keys(object).forEach((propName) => {
        const newPath = ObjectProxy._path(path, propName)
        if (object[propName] !== null && typeof object[propName] === 'object') {
          ObjectProxy._setObject(object[propName], setter, newPath)
        } else {
          setter(newPath, object[propName])
        }
      })
    }
  }

  _isObject(value) {
    return typeof value === 'object' && !Array.isArray(value) && value !== null
  }

  static _path(path, propName) {
    if (path !== null) return path + '.' + propName
    else return propName
  }
}

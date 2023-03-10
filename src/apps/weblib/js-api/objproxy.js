export default class ObjectProxy {

  constructor(setter) {
    this._setter = setter;
  }

  setupProperties(object, path = null) {
    for (const propName in object)
      if(this._isObject(object[propName]))
        this.setupProperties(object[propName], this._path(path, propName));
      else
        this._setupProperty(object, path, propName);
  }

  _setupProperty(object, path, propName) {
    let shadowPropName = '_'+propName;
    let propPath = this._path(path, propName);
    let setter = this._setter;
    object[shadowPropName] = object[propName];
    Object.defineProperty(object, shadowPropName, {
      enumerable: false,
      writable: true
    });
    Object.defineProperty(object, propName, {
        set: function(value) {
            this[shadowPropName] = value;
            setter(propPath, value);
        },
        get: function() {
            return this[shadowPropName];
        },
        enumerable: true
    });
  }

  _isObject(value) {
    return typeof value === 'object' 
      && !Array.isArray(value) &&
      value !== null;
  }

  _path(path, propName) {
    if (path !== null) return path + '.' + propName;
    else return propName;
  }

}


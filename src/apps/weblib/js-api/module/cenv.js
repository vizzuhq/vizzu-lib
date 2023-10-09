import { CError } from './cerror.js'

export class CEnv {
  constructor(wasm, objectRegistry) {
    this._wasm = wasm
    this._objectRegistry = objectRegistry
  }

  _toCString(str) {
    const len = str.length * 4 + 1
    const buffer = this._wasm._malloc(len)
    this._wasm.stringToUTF8(str, buffer, len)
    return buffer
  }

  _fromCString(str) {
    return this._wasm.UTF8ToString(str)
  }

  _get(getter) {
    return this._objectRegistry.get(this._call(getter))
  }

  _call(f) {
    return (...params) => {
      try {
        return f(...params)
      } catch (e) {
        if (Number.isInteger(e)) {
          throw new CError(e, this._wasm)
        } else {
          throw e
        }
      }
    }
  }
}

export class CObject extends CEnv {
  constructor(getId, cenv) {
    super(cenv._wasm, cenv._objectRegistry)
    this.getId = getId
  }

  _call(f) {
    return super._call(f).bind(this, this.getId())
  }
}

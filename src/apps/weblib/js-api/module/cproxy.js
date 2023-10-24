import { CObject } from './cenv.js'
import { cloneObject, iterateObject } from '../utils.js'
export class CProxy extends CObject {
  constructor(getId, cenv, lister, getter, setter) {
    super(getId, cenv)
    this._lister = lister
    this._getter = getter
    this._setter = setter
  }
  set(value) {
    iterateObject(value, this.setParam.bind(this))
  }
  get() {
    return cloneObject(this.listParams.bind(this), this.getParam.bind(this))
  }
  listParams() {
    const clistStr = this._call(this._lister)()
    const listStr = this._fromCString(clistStr)
    const list = JSON.parse(listStr)
    return list
  }
  getParam(path) {
    const cpath = this._toCString(path)
    try {
      const cvalue = this._call(this._getter)(cpath)
      const value = this._fromCString(cvalue)
      // dirty workaround because config.channel.*.set returns already json instead of scalars
      return value.startsWith('[') || value.startsWith('{') ? JSON.parse(value) : value
    } finally {
      this._wasm._free(cpath)
    }
  }
  setParam(path, value) {
    const cpath = this._toCString(path)
    const cvalue = this._toCString(String(value).toString())
    try {
      this._call(this._setter)(cpath, cvalue)
    } finally {
      this._wasm._free(cvalue)
      this._wasm._free(cpath)
    }
  }
}

/// <reference types="../types/cvizzu.d.ts" />

import { CObject } from './cenv'
import { cloneObject, iterateObject } from '../utils'
import { CPointerClosure } from './objregistry'

type Lister = (self: CPointer) => CString
type Getter = (self: CPointer, path: CString) => CString
type Setter = (self: CPointer, path: CString, value: CString) => void

export class CProxy<T> extends CObject {
  _lister: Lister
  _getter: Getter
  _setter: Setter

  constructor(getId: CPointerClosure, cenv: CObject,
    lister: Lister, getter: Getter, setter: Setter) {
    super(getId, cenv)
    this._lister = lister
    this._getter = getter
    this._setter = setter
  }

  set(value: T): void {
    iterateObject(value, this.setParam.bind(this))
  }

  get(): T {
    return cloneObject<T>(this.listParams.bind(this), this.getParam.bind(this))
  }

  listParams() {
    const clistStr = this._call(this._lister)()
    const listStr = this._fromCString(clistStr)
    const list = JSON.parse(listStr)
    return list
  }

  getParam(path: string) {
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

  setParam(path: string, value: string) {
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

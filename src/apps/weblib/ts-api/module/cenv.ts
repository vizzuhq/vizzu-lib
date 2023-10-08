/// <reference types="../types/cvizzu.d.ts" />

import { CError } from './cerror'
import { ObjectRegistry, FnGetter, CPointerClosure } from './objregistry'

export class CEnv {
  _wasm: CVizzu
  _objectRegistry: ObjectRegistry

  constructor(wasm: CVizzu, objectRegistry: ObjectRegistry) {
    this._wasm = wasm
    this._objectRegistry = objectRegistry
  }

  _toCString(str: string): CString {
    const len = str.length * 4 + 1
    const buffer = this._wasm._malloc(len)
    this._wasm.stringToUTF8(str, buffer, len)
    return buffer
  }

  _fromCString(str: CString): string {
    return this._wasm.UTF8ToString(str)
  }

  _getStatic(getter: FnGetter): CPointerClosure {
    return this._objectRegistry.get(this._callStatic(getter))
  }

  _callStatic<T extends any[], R>(f: (...params: T) => R): (...params: T) => R {
    return (...params) => {
      try {
        return f(...params)
      } catch (e: any) {
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
  getId: CPointerClosure

  constructor(getId: CPointerClosure, cenv: CEnv) {
    super(cenv._wasm, cenv._objectRegistry)
    this.getId = getId
  }

  _get(getter: (self: CPointer) => CPointer): CPointerClosure {
    return this._objectRegistry.get(this._call(getter))
  }

  _call<T extends any[], R>(f: (cSelf: CPointer, ...params: T) => R): (...params: T) => R {
    return super._callStatic(f).bind(this, this.getId())
  }
}

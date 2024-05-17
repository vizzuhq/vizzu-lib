import { CPointer, CVizzu, CString } from '../cvizzu.types'

import { CError } from './cerror.js'
import { ObjectRegistry, FnGetter, CPointerClosure } from './objregistry.js'

export class CEnv {
	protected _wasm: CVizzu
	protected _objectRegistry: ObjectRegistry

	constructor(wasm: CVizzu, objectRegistry: ObjectRegistry) {
		this._wasm = wasm
		this._objectRegistry = objectRegistry
	}

	protected _toCString(str: string): CString {
		const len = str.length * 4 + 1
		const buffer = this._wasm._malloc(len)
		this._wasm.stringToUTF8(str, buffer, len)
		return buffer
	}

	protected _fromCString(str: CString): string {
		return this._wasm.UTF8ToString(str)
	}

	protected _getStatic(getter: FnGetter): CPointerClosure {
		return this._objectRegistry.get(this._callStatic(getter))
	}

	protected _callStatic<T extends unknown[], R>(f: (...params: T) => R): (...params: T) => R {
		return (...params) => {
			try {
				return f(...params)
			} catch (e: unknown) {
				if (typeof e === 'number' && Number.isInteger(e)) {
					throw new CError(e, this._wasm)
				} else {
					throw e
				}
			}
		}
	}

	cObjectParams(): readonly [CVizzu, ObjectRegistry] {
		return [this._wasm, this._objectRegistry]
	}
}

export class CObject extends CEnv {
	getId: CPointerClosure

	constructor(getId: CPointerClosure, cenv: CEnv) {
		super(...cenv.cObjectParams())
		this.getId = getId
	}

	protected _get(getter: (self: CPointer) => CPointer): CPointerClosure {
		return this._objectRegistry.get(this._call(getter))
	}

	protected _call<T extends unknown[], R>(
		f: (cSelf: CPointer, ...params: T) => R
	): (...params: T) => R {
		return super._callStatic(f).bind(this, this.getId())
	}
}

import { CObject, CManagedObject } from './cenv.js'

/** Stored Animation object. */
export class CAnimation extends CManagedObject {}

export class CAnimControl extends CObject {
	setParam(path: string, value = ''): void {
		const cpath = this._toCString(path)
		const cvalue = this._toCString(value)
		try {
			this._call(this._wasm._anim_control_setValue)(cpath, cvalue)
		} finally {
			this._wasm._free(cvalue)
			this._wasm._free(cpath)
		}
	}

	getParam(path: string): unknown {
		const cpath = this._toCString(path)
		try {
			const cvalue = this._call(this._wasm._anim_control_getValue)(cpath)
			return this._fromCString(cvalue)
		} finally {
			this._wasm._free(cpath)
		}
	}

	storeAnim(): CAnimation {
		return new CAnimation(this._get(this._wasm._chart_anim_store), this)
	}
}

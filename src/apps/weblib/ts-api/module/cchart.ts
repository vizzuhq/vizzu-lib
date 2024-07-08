import { CString, CFunction, CEventPtr } from '../cvizzu.types'

import * as Anim from '../types/anim.js'
import * as Config from '../types/config.js'
import * as Styles from '../types/styles.js'
import * as Data from '../types/data.js'

import { CManagedObject, CObject, CEnv } from './cenv.js'
import { CPointerClosure } from './objregistry.js'
import { CProxy } from './cproxy.js'
import { CCanvas } from './ccanvas.js'
import { CAnimation } from './canimctrl.js'

import { isIterable } from '../utils.js'

/** Stored Chart object. */
export class Snapshot extends CManagedObject {}

export class CEvent extends CObject {
	preventDefault(): void {
		this._call(this._wasm._event_preventDefault)()
	}
}

class CConfig extends CProxy<Config.Chart> {}
class CStyle extends CProxy<Styles.Chart> {}
class CAnimOptions extends CProxy<Anim.Options> {}

export class CChart extends CManagedObject {
	config: CConfig
	style: CStyle
	computedStyle: CStyle
	animOptions: CAnimOptions
	private _cCanvas?: CCanvas

	constructor(env: CEnv, getId: CPointerClosure) {
		super(getId, env)

		this.config = this._makeConfig()
		this.style = this._makeStyle(false)
		this.computedStyle = this._makeStyle(true)
		this.animOptions = this._makeAnimOptions()
	}

	update(time: number): void {
		this._call(this._wasm._vizzu_update)(time)
	}

	render(cCanvas: CCanvas, width: number, height: number): void {
		this._cCanvas = cCanvas
		this._call(this._wasm._vizzu_render)(cCanvas.getId(), width, height, 0)
	}

	animate(callback: (ok: boolean) => void): void {
		const callbackPtr = this._wasm.addFunction((ok: boolean) => {
			callback(ok)
			this._wasm.removeFunction(callbackPtr)
		}, 'vi')
		this._call(this._wasm._chart_animate)(callbackPtr)
	}

	storeSnapshot(): Snapshot {
		return new Snapshot(this._get(this._wasm._chart_store), this)
	}

	restoreSnapshot(snapshot: Snapshot): void {
		this._call(this._wasm._chart_restore)(snapshot.getId())
	}

	restoreAnim(animation: CAnimation): void {
		this._call(this._wasm._chart_anim_restore)(animation.getId())
	}

	setKeyframe(): void {
		this._call(this._wasm._chart_setKeyframe)()
	}

	addEventListener<T>(eventName: string, func: (event: CEvent, param: T) => void): CFunction {
		const wrappedFunc = (eventPtr: CEventPtr, param: CString): void => {
			const eventObj = new CEvent(() => eventPtr, this)
			func(eventObj, JSON.parse(this._fromCString(param)))
		}
		const cfunc = this._wasm.addFunction(wrappedFunc, 'vii')
		const cname = this._toCString(eventName)
		try {
			this._call(this._wasm._addEventListener)(cname, cfunc)
		} finally {
			this._wasm._free(cname)
		}
		return cfunc
	}

	removeEventListener(eventName: string, cfunc: CFunction): void {
		const cname = this._toCString(eventName)
		try {
			this._call(this._wasm._removeEventListener)(cname, cfunc)
		} finally {
			this._wasm._free(cname)
		}
		this._wasm.removeFunction(cfunc)
	}

	pointerdown(pointerId: number, x: number, y: number): void {
		if (!this._cCanvas) return
		this._call(this._wasm._vizzu_pointerDown)(this._cCanvas.getId(), pointerId, x, y)
	}

	pointermove(pointerId: number, x: number, y: number): void {
		if (!this._cCanvas) return
		this._call(this._wasm._vizzu_pointerMove)(this._cCanvas.getId(), pointerId, x, y)
	}

	pointerup(pointerId: number, x: number, y: number): void {
		if (!this._cCanvas) return
		this._call(this._wasm._vizzu_pointerUp)(this._cCanvas.getId(), pointerId, x, y)
	}

	pointerleave(pointerId: number): void {
		if (!this._cCanvas) return
		this._call(this._wasm._vizzu_pointerLeave)(this._cCanvas.getId(), pointerId)
	}

	wheel(delta: number): void {
		if (!this._cCanvas) return
		this._call(this._wasm._vizzu_wheel)(this._cCanvas.getId(), delta)
	}

	getString(text: CString): string {
		return this._wasm.UTF8ToString(text)
	}

	private _makeConfig(): CConfig {
		return new CConfig(
			this.getId,
			this,
			this._wasm._chart_getList,
			this._wasm._chart_getValue,
			this._wasm._chart_setValue,
			(value: unknown): value is Record<string, unknown> =>
				isIterable(value) && !this._isSeriesDescriptor(value),
			(value: unknown): string => {
				// workaround: we should be able to pass series descriptor as two string
				if (this._isSeriesDescriptor(value)) {
					return value.aggregator ? `${value.aggregator}(${value.name})` : value.name
				} else {
					return String(value).toString()
				}
			},
			(path: string, value: string): unknown => {
				// workaround because channel.*.set returns already json instead of scalars
				if (path.startsWith('channels.') && path.endsWith('.set')) {
					return JSON.parse(value).map((v: string) => this._toSeriesDescriptor(v))
				} else return value
			}
		)
	}

	private _toSeriesDescriptor(value: string): Data.SeriesDescriptor {
		const pattern = /^(\w+)\((.*?)\)$/
		const match = value.match(pattern)
		if (match) {
			return {
				name: match[2]!,
				aggregator: match[1]! as Data.AggregatorType
			}
		} else {
			return { name: value }
		}
	}

	private _isSeriesDescriptor(value: unknown): value is Data.SeriesDescriptor {
		return typeof value === 'object' && value !== null && 'name' in value
	}

	private _makeStyle(computed: boolean): CStyle {
		return new CStyle(
			this.getId,
			this,
			this._wasm._style_getList,
			(ptr, path) => this._wasm._style_getValue(ptr, path, computed),
			this._wasm._style_setValue
		)
	}

	private _makeAnimOptions(): CAnimOptions {
		return new CAnimOptions(
			this.getId,
			this,
			() => 0,
			() => 0,
			this._wasm._anim_setValue
		)
	}
}

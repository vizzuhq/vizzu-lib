import { CEnv, CObject } from './cenv.js'
import { CPointerClosure } from './objregistry.js'
import { CColorGradient } from './ccolorgradient.js'
import { CString, CColorGradientPtr } from '../cvizzu.types'

export abstract class CCanvas extends CObject {
	constructor(env: CEnv, getId: CPointerClosure) {
		super(getId, env)
	}

	abstract setFontStyle(font: string): void
	abstract drawText(x: number, y: number, sizex: number, sizey: number, text: string): void
	abstract setGradient(
		x1: number,
		y1: number,
		x2: number,
		y2: number,
		gradient: CColorGradient
	): void

	setFont(font: CString): void {
		this.setFontStyle(this._getString(font))
	}

	text(x: number, y: number, sizex: number, sizey: number, text: CString): void {
		this.drawText(x, y, sizex, sizey, this._getString(text))
	}

	setBrushGradient(
		x1: number,
		y1: number,
		x2: number,
		y2: number,
		stopCount: number,
		stops: CColorGradientPtr
	): void {
		this.setGradient(x1, y1, x2, y2, this._getColorGradient(stops, stopCount))
	}

	private _getColorGradient(stops: CColorGradientPtr, stopCount: number): CColorGradient {
		return new CColorGradient(this, stops, stopCount)
	}

	private _getString(text: CString): string {
		return this._wasm.UTF8ToString(text)
	}
}

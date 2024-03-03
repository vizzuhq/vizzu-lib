import { CEnv } from './cenv.js'
import { CString, CPointer, TextSize } from '../cvizzu.types'
import { Point } from '../geom.js'

export abstract class CTextSize extends CEnv implements TextSize {
	constructor(cenv: CEnv) {
		super(...cenv.cObjectParams())
	}

	abstract getTextSize(font: string, text: string): Point

	getSize(cfont: CString, ctext: CString, sizeX: CPointer, sizeY: CPointer): void {
		const font = this._wasm.UTF8ToString(cfont)
		const text = this._wasm.UTF8ToString(ctext)
		const size = this.getTextSize(font, text)
		this._wasm.setValue(sizeX, size.x, 'double')
		this._wasm.setValue(sizeY, size.y, 'double')
	}
}

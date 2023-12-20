import { CColorGradientPtr } from '../cvizzu.types.js'
import { CEnv, CObject } from './cenv.js'

interface ColorStop {
	offset: number
	color: string
}

export class CColorGradient extends CObject {
	stops: ColorStop[]

	constructor(env: CEnv, stopsPtr: CColorGradientPtr, stopCount: number) {
		super(() => stopsPtr, env)
		this.stops = this._getStops(stopCount)
	}

	private _getStops(stopCount: number): ColorStop[] {
		const stopList = []
		const ptr = this.getId()
		for (let i = 0; i < stopCount * 40; i += 40) {
			stopList.push({
				offset: this._wasm.getValue(ptr + i, 'double'),
				color:
					'rgba(' +
					this._wasm.getValue(ptr + i + 8, 'double') * 255 +
					',' +
					this._wasm.getValue(ptr + i + 16, 'double') * 255 +
					',' +
					this._wasm.getValue(ptr + i + 24, 'double') * 255 +
					',' +
					this._wasm.getValue(ptr + i + 32, 'double') +
					')'
			})
		}
		return stopList
	}
}

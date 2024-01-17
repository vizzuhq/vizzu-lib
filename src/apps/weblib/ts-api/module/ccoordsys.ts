import { CPointPtr } from '../cvizzu.types.js'
import { Point } from '../geom.js'
import { CObject } from './cenv.js'

export class CCoordSystem extends CObject {
	toCanvasCoords(point: Point): Point {
		const ptr = this._call(this._wasm._chart_relToCanvasCoords)(point.x, point.y)
		return this._getPoint(ptr)
	}

	toRelCoords(point: Point): Point {
		const ptr = this._call(this._wasm._chart_canvasToRelCoords)(point.x, point.y)
		return this._getPoint(ptr)
	}

	private _getPoint(ptr: CPointPtr): Point {
		return {
			x: this._wasm.getValue(ptr, 'double'),
			y: this._wasm.getValue(ptr + 8, 'double')
		}
	}
}

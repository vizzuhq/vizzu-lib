import { CCoordSystem } from '../module/ccoordsys'
import { Plugin, PluginApi } from '../plugins'
import { Point } from '../geom'

export interface CoordSystemApi extends PluginApi {
	toCanvas(point: Point): Point
	toRelative(point: Point): Point
}

export class CoordSystem implements Plugin {
	private _cCoordSys: CCoordSystem

	constructor(cCoordSys: CCoordSystem) {
		this._cCoordSys = cCoordSys
	}

	meta = {
		name: 'coordSystem'
	}

	api: CoordSystemApi = {
		toCanvas: (p: Point) => {
			return this._cCoordSys.toCanvasCoords(p)
		},
		toRelative: (p: Point) => {
			return this._cCoordSys.toRelCoords(p)
		}
	}
}

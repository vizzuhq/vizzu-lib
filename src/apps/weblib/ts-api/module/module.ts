import { CVizzu } from '../cvizzu.types'

import { CPointerClosure, ObjectRegistry } from './objregistry.js'
import { CEnv } from './cenv.js'
import { CData } from './cdata.js'
import { CChart } from './cchart.js'
import { CCanvas } from './ccanvas.js'
import { CAnimControl } from './canimctrl.js'
import { CCoordSystem } from './ccoordsys.js'
import { Canvas } from './canvas.js'

export class Module extends CEnv {
	constructor(wasm: CVizzu) {
		super(wasm, new ObjectRegistry(wasm._object_free))
		const context2D = (<HTMLCanvasElement>document.createElement('canvas')).getContext('2d')
		if (!context2D) throw new Error('Failed to get 2D context')
		this._wasm.measureCanvas = context2D
		this._wasm.canvases = {}
		this.setLogging(false)
	}

	registerRenderer(cCanvas: CCanvas & Canvas): void {
		this._wasm.canvases[cCanvas.getId()] = cCanvas
	}

	unregisterRenderer(cCanvas: CCanvas & Canvas): void {
		delete this._wasm.canvases[cCanvas.getId()]
	}

	version(): string {
		return this._wasm.UTF8ToString(this._wasm._vizzu_version())
	}

	setLogging(enabled: boolean): void {
		this._callStatic(this._wasm._vizzu_setLogging)(enabled)
	}

	getData(cChart: CChart): CData {
		return new CData(cChart.getId, this)
	}

	getCoordSystem(cChart: CChart): CCoordSystem {
		return new CCoordSystem(cChart.getId, this)
	}

	getAnimControl(cChart: CChart): CAnimControl {
		return new CAnimControl(cChart.getId, this)
	}

	createChart(): CChart {
		return new CChart(this, this._getStatic(this._wasm._vizzu_createChart))
	}

	createCanvas<T extends CCanvas>(ctor: new (env: CEnv, getId: CPointerClosure) => T): T {
		return new ctor(this, this._getStatic(this._wasm._vizzu_createCanvas))
	}
}

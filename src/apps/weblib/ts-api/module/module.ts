import { CPointer, CVizzu } from '../cvizzu.types';
import * as Data from '../types/data.js'

import { ObjectRegistry } from './objregistry.js'
import { CEnv } from './cenv.js'
import { CData } from './cdata.js'
import { CChart } from './cchart.js'
import { CCanvas } from './ccanvas.js'
import { CAnimControl } from './canimctrl.js'
import { CCoordSystem } from './ccoordsys.js'
import { Canvas } from './canvas.js'
import { Chart } from './chart.js'
import { AggregatorType } from "../types/data.js";

export class Module extends CEnv {
	constructor(wasm: CVizzu) {
		super(wasm, new ObjectRegistry(wasm._object_free))
		const context2D = (<HTMLCanvasElement>document.createElement('canvas')).getContext('2d')
		if (!context2D) throw new Error('Failed to get 2D context')
		this._wasm.measureCanvas = context2D
		this._wasm.canvases = {}
		this._wasm.charts = {}
		this.setLogging(false)
	}

	registerChart(cChart: CChart, chart: Chart): void {
		this._wasm.charts[cChart.getId()] = chart
	}

	unregisterChart(cChart: CChart): void {
		delete this._wasm.charts[cChart.getId()]
	}

	registerRenderer(cCanvas: CCanvas, canvas: Canvas): void {
		this._wasm.canvases[cCanvas.getId()] = canvas
	}

	unregisterRenderer(cCanvas: CCanvas): void {
		delete this._wasm.canvases[cCanvas.getId()]
	}

	version(): string {
		return this._wasm.UTF8ToString(this._wasm._vizzu_version())
	}

	setLogging(enabled: boolean): void {
		this._callStatic(this._wasm._vizzu_setLogging)(enabled)
	}

	getCoordSystem(cChart: CChart): CCoordSystem {
		return new CCoordSystem(cChart.getId, this)
	}

	getAnimControl(cChart: CChart): CAnimControl {
		return new CAnimControl(cChart.getId, this)
	}

	createData(): CData {
		return new CData(this._getStatic(this._wasm._vizzu_createData), this)
	}

	createExternalData(isDimension: Map<string, boolean>, toInfo: Map<string, Map<string, string>>,
										 aggregatorFunction: (data: CData,
																					filt1: CPointer,
																					filt2: CPointer,
																					grouping: Data.SeriesList,
																					aggregating: Data.SeriesList) => string[]
										 ): CData {
		const callbackPtrs: [CPointer, CPointer, CPointer, CPointer, CPointer] = [0, 0, 0, 0, 0]
		const stringDeleter = (ptr: CPointer): void => {
			this._wasm._free(ptr)
		}
		callbackPtrs[0] = this._wasm.addFunction(stringDeleter, 'vi')

		const seriesMeta = (series: CPointer): boolean => {
			const res = isDimension.get(this._fromCString(series))
			if (res === undefined) throw new Error('Unknown series')
			return res
		}
		callbackPtrs[1] = this._wasm.addFunction(seriesMeta, 'ii')

		const seriesInfo = (series: CPointer, key: CPointer): CPointer => {
			const seriesName = this._fromCString(series)
			const keyName = this._fromCString(key)
			const info = toInfo.get(seriesName)
			if (!info) throw new Error('Unknown series')
			return this._toCString(info.get(keyName) || '')
		}
		callbackPtrs[2] = this._wasm.addFunction(seriesInfo, 'iii')

		const aggregator = (dataPtr: CPointer, filt1: CPointer, filt2: CPointer,
												grouping: number, groupingList: CPointer,
												aggregating: number, aggregatingNames: CPointer,
												aggregatingFunctions: CPointer, outputNames: CPointer): void => {
			const data = new CData((): CPointer => dataPtr, this, false)

			const groupingArray: Data.SeriesList = new Array(grouping)
			for (let i = 0; i < grouping; i++) {
				groupingArray[i] = {
					name: this._fromCString(this._wasm.getValue(groupingList + i * 4, 'i8*'))
				}
			}
			const aggregatingArray: Data.SeriesList = new Array(aggregating)
			for (let i = 0; i < aggregating; i++) {
				aggregatingArray[i] = {
					name: this._fromCString(this._wasm.getValue(aggregatingNames + i * 4, 'i8*')),
					aggregator: this._fromCString(this._wasm.getValue(aggregatingFunctions + i * 4, 'i8*')) as AggregatorType
				}
				this._wasm.setValue(outputNames + i * 4, 0, 'i8*')
			}

			const aggregated = aggregatorFunction(data, filt1, filt2, groupingArray, aggregatingArray)
			for (let i = 0; i < aggregating; i++) {
				const val = aggregated[i]
				if (val) {
					this._wasm.setValue(outputNames + i * 4, this._toCString(val), 'i8*')
				}
			}
		}
		callbackPtrs[3] = this._wasm.addFunction(aggregator, 'viiiiiiiii')

		const deleter = (): void => {
			for (const ptr of callbackPtrs) {
				this._wasm.removeFunction(ptr)
			}
		}
		callbackPtrs[4] = this._wasm.addFunction(deleter, 'v')

		const cPointer = this._callStatic(this._wasm._vizzu_createExternalData)(...callbackPtrs)
		return new CData((): CPointer => cPointer, this)
	}

	createChart(data : CData): CChart {
		const cPointer = this._callStatic(this._wasm._vizzu_createChart)(data.getId())
		return new CChart(this, (): CPointer => cPointer)
	}

	createCanvas(): CCanvas {
		return new CCanvas(this, this._getStatic(this._wasm._vizzu_createCanvas))
	}
}

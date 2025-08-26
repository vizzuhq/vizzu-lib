import * as D from './types/data.js'

import { CData } from './module/cdata.js'
import { CRecord, CChart } from './module/cchart.js'
import { DataRecord } from './datarecord.js'
import { Mirrored } from './tsutils.js'
import { CPointer } from './cvizzu.types'

type DataTypes = D.DimensionValue | D.MeasureValue
export class Data {
	private readonly _cData: CData
	private readonly _cChart: CChart | undefined
	private readonly _dataOwned: boolean
	private readonly _filters: Map<CPointer, D.OutFilterCallback>

	constructor(cData: CData, cChart: CChart | undefined = undefined, dataOwned: boolean = true) {
		this._cData = cData
		this._cChart = cChart
		this._dataOwned = dataOwned
		this._filters = new Map()
	}

	detach(): void {
		if (this._dataOwned) {
			this._cData.free()
		}
	}

	getMetaInfo(): Mirrored<D.Metainfo> {
		return this._cData.getMetaInfo()
	}

	getFilterByPtr(ptr: CPointer): D.OutFilterCallback | null {
		return this._filters.get(ptr) ?? null
	}

	set(obj?: D.TableBySeries | D.TableByRecords): void {
		if (!obj) {
			return
		}

		if (obj.series) {
			if (!Array.isArray(obj.series)) {
				throw new Error('data series field is not an array')
			}

			for (const series of obj.series) {
				this._setSeries(series)
			}
		}

		if ('records' in obj) {
			if (!Array.isArray(obj.records)) {
				throw new Error('data records field is not an array')
			}

			const seriesList = this._cData.getMetaInfo().series
			for (const record of obj.records) {
				this._addRecord(record, seriesList)
			}
		}

		if (obj.filter || obj.filter === null) {
			this._setFilter(obj.filter)
		}
	}

	private _addRecord(
		record: D.ValueArray | D.Record,
		seriesList: Mirrored<D.SeriesInfo[]>
	): void {
		if (!Array.isArray(record)) {
			if (typeof record === 'object' && record !== null) {
				record = this._recordObjectToArray(record, seriesList)
			} else throw new Error('data record is not an array or object')
		}
		this._cData.addRecord(record)
	}

	private _recordObjectToArray(
		record: D.Record,
		seriesList: Mirrored<D.SeriesInfo[]>
	): D.Value[] {
		const result = [] as D.Value[]

		seriesList.forEach((series) => {
			if (series.name in record) {
				result.push(record[series.name] || this._default(series.type))
			} else {
				result.push(this._default(series.type))
			}
		})

		return result
	}

	private _default(type?: string): D.Value {
		return type === 'measure' ? 0 : ''
	}

	private _setSeries(series: D.Series): void {
		if (!series.name) {
			throw new Error('missing series name')
		}

		if (this._isIndexedDimension(series)) {
			this._validateIndexedDimension(series)
			this._addDimension(
				series.name,
				series.values ?? [],
				series.categories,
				series.isContiguous ?? false
			)
		} else {
			const values = series?.values ?? ([] as DataTypes[])
			const seriesType = series?.type ?? this._detectType(values)

			if (this._isDetectedDimension(series, seriesType)) {
				const { indexes, categories } = this._convertDimension(values)
				this._addDimension(series.name, indexes, categories, series.isContiguous ?? false)
			} else if (this._isMeasure(series, seriesType)) {
				if (!series.unit) series.unit = ''
				this._addMeasure(series.name, series.unit, values)
			} else {
				const seriesBase = series as D.SeriesBase
				throw new Error('invalid series type: ' + seriesBase.type)
			}
		}
	}

	private _isIndexedDimension(series: D.Series): series is D.IndexDimension {
		if (!('categories' in series && Array.isArray(series.categories))) {
			return false
		}
		return true
	}

	private _isDetectedDimension(series: D.Series, type: string | null): series is D.Dimension {
		return (
			type === 'dimension' ||
			('isContiguous' in series && typeof series.isContiguous === 'boolean')
		)
	}

	private _isMeasure(series: D.Series, type: string | null): series is D.Measure {
		return type === 'measure' || ('unit' in series && typeof series.unit === 'string')
	}

	private _validateIndexedDimension(series: D.IndexDimension): void {
		if (series.values && Array.isArray(series.values)) {
			const max = series.categories.length
			for (const value of series.values) {
				if (value === null) {
					continue
				}

				const numberValue = Number(value)

				if (!Number.isInteger(numberValue)) {
					throw new Error('invalid category value (not an int): ' + numberValue)
				}
				if (numberValue < 0) {
					throw new Error('invalid category value (negative): ' + numberValue)
				}
				if (numberValue >= max) {
					throw new Error('invalid category value (out of range): ' + numberValue)
				}
			}
		}
		if (new Set(series.categories).size !== series.categories.length) {
			throw new Error('duplicate category values')
		}
	}

	private _detectType(values: DataTypes[]): D.SeriesType | null {
		if (Array.isArray(values) && values.length) {
			if (typeof values[0] === 'number' || values[0] === null) {
				return 'measure'
			} else if (typeof values[0] === 'string') {
				return 'dimension'
			}
		}
		return null
	}

	private _addDimension(
		name: string,
		indexes: number[],
		categories: string[],
		isContiguous: boolean
	): void {
		if (typeof name !== 'string') {
			throw new Error('first parameter should be string')
		}

		if (!(indexes instanceof Array)) {
			throw new Error('second parameter should be an array')
		}

		if (!(categories instanceof Array)) {
			throw new Error('third parameter should be an array')
		}

		if (typeof isContiguous !== 'boolean') {
			throw new Error('fourth parameter should be boolean')
		}

		if (!this._isStringArray(categories)) {
			throw new Error('third parameter should be an array of strings')
		}

		if (!this._isDimensionIndex(indexes)) {
			throw new Error('the measure index array element should be number')
		}

		this._cData.addDimension(name, indexes, categories, isContiguous)
	}

	private _isStringArray(values: unknown[]): values is string[] {
		for (const value of values) {
			if (typeof value !== 'string') {
				return false
			}
		}
		return true
	}

	private _isDimensionIndex(indexes: unknown[]): indexes is number[] {
		for (const index of indexes) {
			const numberValue = Number(index)

			if (!Number.isInteger(numberValue) || numberValue < 0) {
				return false
			}
		}
		return true
	}

	private _convertDimension(values: DataTypes[]): { categories: string[]; indexes: number[] } {
		const uniques = new Map()
		const categories: string[] = []
		const indexes = new Array(values.length)

		for (let index = 0; index < values.length; index++) {
			const value = values[index]
			if (typeof value !== 'string') {
				continue
			}
			let uniqueIndex = uniques.get(value)

			if (uniqueIndex === undefined) {
				uniqueIndex = categories.length
				uniques.set(value, uniqueIndex)
				categories.push(value)
			}

			indexes[index] = uniqueIndex
		}

		return { categories, indexes }
	}

	private _addMeasure(name: string, unit: string, values: unknown[]): void {
		if (typeof name !== 'string') {
			throw new Error("'name' parameter should be string")
		}

		if (typeof unit !== 'string') {
			throw new Error("'unit' parameter should be string")
		}

		if (!(values instanceof Array)) {
			throw new Error("'values' parameter should be an array")
		}

		const numbers = values.map((value) => Number(value))

		this._cData.addMeasure(name, unit, numbers)
	}

	private _setFilter(filter: D.FilterCallback | D.OutFilterCallback | null): void {
		if (this._cChart === undefined) {
			throw new Error('chart is not attached')
		}
		if (filter === null) {
			this._cChart.setFilter(null)
		} else if (typeof filter === 'function' && filter.length == 1) {
			const callback = (cRecord: CRecord): boolean =>
				(filter as D.FilterCallback)(new DataRecord(cRecord))
			this._cChart.setFilter(callback)
		} else if (typeof filter === 'function' && filter.length == 0) {
			this._filters.set(
				this._cChart.setFilter(
					(): boolean => true,
					(ptr: CPointer) => {
						this._filters.delete(ptr)
					}
				),
				filter as D.OutFilterCallback
			)
		} else {
			throw new Error('data filter is not a function or null')
		}
	}
}

import * as D from './types/data.js'

import { CRecord, CData } from './module/cdata.js'
import { DataRecord } from './datarecord.js'
import { Mirrored } from './tsutils.js'

export class Data {
	private _cData: CData

	constructor(cData: CData) {
		this._cData = cData
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

		const values = series.values ? series.values : ([] as D.Values)

		if (this._isIndexedDimension(series)) {
			this._validateIndexedDimension(series)
			this._addDimension(series.name, values as number[], series.categories)
		}
		else {
			const seriesType = series.type ? series.type : this._detectType(values)
			if (seriesType === 'dimension') {
				const { indexes, categories } = this._convertDimension(values)
				this._addDimension(series.name, indexes, categories)
			} else if (seriesType === 'measure') {
				if (!series.unit) series.unit = ''
				this._addMeasure(series.name, series.unit, values)
			} else {
				throw new Error('invalid series type: ' + series.type)
			}
		}
	}

	private _isIndexedDimension(series: D.Series): series is D.IndexDimension {
		if (!('categories' in series && Array.isArray(series.categories))) {
			return false
		}
		return true
	}

	private _validateIndexedDimension(series: D.IndexDimension): void {
		if (series.values && Array.isArray(series.values)) {

			let max = 0
			for (const value of series.values) {
				if (value === null) {
					continue
				}

				const numberValue = Number(value)

				if (Number.isInteger(numberValue)) {
					throw new Error('invalid category values')
				}
				if (numberValue < 0) {
					throw new Error('invalid category values')
				}
				if (numberValue > max) {
					max = numberValue
				}
			}

			if (max && max >= series.categories.length) {
				throw new Error('invalid category values')
			}
		}
		if ((new Set(series.categories)).size !== series.categories.length) {
			throw new Error('duplicate category values')
		}
	}

	private _detectType(values: D.Values): D.SeriesType | null {
		if (Array.isArray(values) && values.length) {
			if (typeof values[0] === 'number' || values[0] === null) {
				return 'measure'
			} else if (typeof values[0] === 'string') {
				return 'dimension'
			}
		}
		return null
	}

	private _addDimension(name: string, indexes: number[], categories: string[]): void {
		if (typeof name !== 'string') {
			throw new Error('first parameter should be string')
		}

		if (!(indexes instanceof Array)) {
			throw new Error('second parameter should be an array')
		}

		if (!(categories instanceof Array)) {
			throw new Error('third parameter should be an array')
		}

		if (!this._isStringArray(categories)) {
			throw new Error('third parameter should be an array of strings')
		}

		if (!this._isMeasureArray(indexes)) {
			throw new Error('the mesaure index array element should be number')
		}

		this._cData.addDimension(name, indexes, categories)
	}

	private _isStringArray(values: unknown[]): values is string[] {
		for (const value of values) {
			if (typeof value !== 'string') {
				return false
			}
		}
		return true
	}

	private _isMeasureArray(values: unknown[]): values is string[] {
		for (const value of values) {
			if (value !== null && typeof value !== 'number') {
				return false
			}
		}
		return true
	}

	private _convertDimension(values: D.Values): { categories: string[], indexes: number[] } {
		const uniques = new Map();
		const categories: string[] = [];
		const indexes = new Array(values.length);

		for (let index = 0; index < values.length; index++) {
			const value = values[index];
			if (typeof value !== 'string') {
				continue;
			}
			let uniqueIndex = uniques.get(value);

			if (uniqueIndex === undefined) {
				uniqueIndex = categories.length;
				uniques.set(value, uniqueIndex);
				categories.push(value).toString();
			}

			indexes[index] = uniqueIndex;
		};

		return { categories, indexes };
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

	private _setFilter(filter: D.FilterCallback | null): void {
		if (typeof filter === 'function') {
			const callback = (cRecord: CRecord): boolean => filter(new DataRecord(cRecord))
			this._cData.setFilter(callback)
		} else if (filter === null) {
			this._cData.setFilter(null)
		} else {
			throw new Error('data filter is not a function or null')
		}
	}
}

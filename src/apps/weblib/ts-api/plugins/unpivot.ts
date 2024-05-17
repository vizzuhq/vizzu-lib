import * as Data from '../types/data.js'

type CubeRow = Data.MeasureValue[] | CubeRow[]

/** Defines a data series of the data cube, and contains a particular variable's
  values in the data cube and meta info about that variable. */
interface CubeData extends Data.SeriesBase {
	/** A nested array that contains the values of the data series. Nesting 
  level should match the number of data cube dimensions. */
	values: CubeRow
}

/** N dimensional data cude */
export interface DataCube extends Data.Filter {
	/** The list of the dimensions of the data cube. */
	dimensions: Data.Series[]
	/** The list of measures of the data cube. */
	measures: CubeData[]
}

const assert = (condition: boolean, message: string): void => {
	if (!condition) {
		throw new Error('Assert failed: ' + message)
	}
}

const assertArray = (data: DataCube, array: unknown[], index: number): void => {
	assert(Array.isArray(array), 'array is not a list')
	try {
		if (array.length !== data.dimensions[index]!.values!.length) {
			throw new Error('Assert failed')
		}
	} catch (err) {
		assert(false, 'dimensions are not the same')
	}
	array.forEach((item) => {
		if (Array.isArray(item)) {
			assertArray(data, item, index - 1)
		}
	})
}

export default class UnPivot {
	static isPivot(data: Data.Set | DataCube | undefined): boolean {
		return (
			data !== undefined &&
			(('dimensions' in data && data.dimensions !== undefined) ||
				('measures' in data && data.measures !== undefined))
		)
	}

	static convert(data: DataCube): Data.TableBySeries {
		assert(
			typeof data === 'object' && data !== null && !Array.isArray(data),
			'data is not an object'
		)
		assert('dimensions' in data, 'data.dimensions is requreid')
		assert('measures' in data, 'data.measures is requreid')

		const convertedData: Data.TableBySeries = (({
			// eslint-disable-next-line @typescript-eslint/no-unused-vars
			dimensions,
			// eslint-disable-next-line @typescript-eslint/no-unused-vars
			measures,
			...o
		}: DataCube): Data.TableBySeries => Object.assign(o, { series: [] }))(data)

		let dimensionsProduct = 1
		assert(Array.isArray(data.dimensions), 'data.dimensions is not a list')
		for (let i = 0; i < data.dimensions.length; i++) {
			const item = data.dimensions[i]!
			assert(
				typeof item === 'object' && item !== null && !Array.isArray(item),
				'data.dimensions.item is not an object'
			)
			assert('name' in item, 'data.dimensions.item.name is requreid')
			assert(
				typeof item.type === 'undefined' || item.type === 'dimension',
				'data.dimensions.item.type is invalid'
			)
			assert('values' in item, 'data.dimensions.item.values is requreid')
			assert(Array.isArray(item.values), 'data.dimensions.item.values is not a list')
			assert(item.values!.length !== 0, 'data.dimensions.item.values length is zero')
			dimensionsProduct *= item.values!.length
		}

		let dimensionsProductProcessed = 1
		for (let i = 0; i < data.dimensions.length; i++) {
			const item = data.dimensions[i]!
			let values = [] as string[]
			const valuesItem = [] as string[]
			item.values!.forEach((value) => {
				for (let b = 1; b <= dimensionsProductProcessed; b++) {
					valuesItem.push(value as string)
				}
			})
			dimensionsProductProcessed *= item.values!.length
			for (let a = 1; a <= dimensionsProduct / dimensionsProductProcessed; a++) {
				values = values.concat(valuesItem)
			}

			const seriesItem: Data.Dimension = {
				name: item.name,
				type: 'dimension',
				values
			}
			convertedData.series.push(seriesItem)
		}

		assert(
			typeof data.measures === 'object' && data.measures !== null,
			'data.measures is not a list or an object'
		)
		if (!Array.isArray(data.measures)) {
			data.measures = [data.measures]
		}
		for (let i = 0; i < data.measures.length; i++) {
			const item = data.measures[i]!
			assert(
				typeof item === 'object' && item !== null && !Array.isArray(item),
				'data.measures.item is not an object'
			)
			assert('name' in item, 'data.measures.item.name is requreid')
			assert('values' in item, 'data.measures.item.values is requreid')
			assert(Array.isArray(item.values), 'data.measures.item.values is not a list')
			assertArray(data, item.values, data.dimensions.length - 1)
			const seriesItem: {
				name: string
				type?: 'dimension' | 'measure' | undefined
				values: unknown[]
			} = {
				name: item.name,
				type: item.type,
				values: (item.values as unknown[]).flat(Infinity)
			}
			if (typeof seriesItem.type === 'undefined') {
				delete seriesItem.type
			}
			assert(seriesItem.values.length === dimensionsProduct, 'dimensions are not the same')
			convertedData.series.push(seriesItem as Data.Series)
		}

		return convertedData
	}
}

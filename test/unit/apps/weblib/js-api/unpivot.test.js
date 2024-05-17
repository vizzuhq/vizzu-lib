import UnPivot from '../../../../../dist/plugins/unpivot.js'
import Data1D from './unpivot.test.data/unpivot.test.data.1D.js'
import Data3D from './unpivot.test.data/unpivot.test.data.3D.js'

describe('UnPivot.isPivot(data)', () => {
	test('true - 1', () => {
		const data = { dimensions: null, measures: null }
		expect(UnPivot.isPivot(data)).toBeTruthy()
	})

	test('true - 2', () => {
		const data = { measures: null }
		expect(UnPivot.isPivot(data)).toBeTruthy()
	})

	test('true - 3', () => {
		const data = { dimensions: null }
		expect(UnPivot.isPivot(data)).toBeTruthy()
	})

	test('true - 4', () => {
		expect(UnPivot.isPivot(Data3D.data_3d_1.input)).toBeTruthy()
	})

	test('false - 1', () => {
		const data = {}
		expect(UnPivot.isPivot(data)).toBeFalsy()
	})

	test('false - 2', () => {
		const data = { dimensions: undefined }
		expect(UnPivot.isPivot(data)).toBeFalsy()
	})

	test('false - 3', () => {
		const data = { measures: undefined }
		expect(UnPivot.isPivot(data)).toBeFalsy()
	})

	test('false - 4', () => {
		const data = { dimensions: undefined, measures: undefined }
		expect(UnPivot.isPivot(data)).toBeFalsy()
	})
})

describe('UnPivot.convert(data)', () => {
	describe('1D', () => {
		test('filter', () => {
			const output = UnPivot.convert(Data1D.data_1d_1.input)
			expect(output.filter.toString()).toMatch(Data1D.data_1d_1.output.filter.toString())
		})

		test('convert_1D_1 - measures = list', () => {
			const output = UnPivot.convert(Data1D.data_1d_1.input)
			expect(JSON.stringify(output.series)).toMatch(
				JSON.stringify(Data1D.data_1d_1.output.series)
			)
		})

		test('convert_1D_2 - measures = object', () => {
			const output = UnPivot.convert(Data1D.data_1d_2.input)
			expect(JSON.stringify(output.series)).toMatch(
				JSON.stringify(Data1D.data_1d_2.output.series)
			)
		})

		test('convert_1D_3 - data.dimensions.item.type = undefined', () => {
			const output = UnPivot.convert(Data1D.data_1d_3.input)
			expect(JSON.stringify(output.series)).toMatch(
				JSON.stringify(Data1D.data_1d_3.output.series)
			)
		})

		test('convert_1D_4 - data.dimensions.item.type = null', () => {
			const output = UnPivot.convert(Data1D.data_1d_4.input)
			expect(JSON.stringify(output.series)).toMatch(
				JSON.stringify(Data1D.data_1d_4.output.series)
			)
		})

		test('convert_1D_5 - data.dimensions.item.type = dimension', () => {
			const output = UnPivot.convert(Data1D.data_1d_5.input)
			expect(JSON.stringify(output.series)).toMatch(
				JSON.stringify(Data1D.data_1d_5.output.series)
			)
		})
	})

	describe('3D', () => {
		test('convert_3D_1', () => {
			const output = UnPivot.convert(Data3D.data_3d_1.input)
			expect(JSON.stringify(output.series)).toMatch(
				JSON.stringify(Data3D.data_3d_1.output.series)
			)
		})

		test('filter', () => {
			const output = UnPivot.convert(Data3D.data_3d_1.input)
			expect(output.filter.toString()).toMatch(Data3D.data_3d_1.output.filter.toString())
		})
	})
})

describe('UnPivot.convert(data) - assert data', () => {
	describe('typeof(data) != object', () => {
		describe('data = undefined', () => {
			test('non-existent', () => {
				expect(() => UnPivot.convert()).toThrow('Assert failed: data is not an object')
			})

			test('undefined', () => {
				let data
				expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
			})
		})

		test('data = string', () => {
			const data = 'data'
			expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
		})

		describe('data = number', () => {
			test('1', () => {
				const data = 1
				expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
			})

			test('1.11', () => {
				const data = 1.11
				expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
			})

			test('Infinity', () => {
				const data = 1 / 0
				expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
			})

			test('-Infinity', () => {
				const data = -1 / 0
				expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
			})
		})

		test('data = boolean', () => {
			const data = true
			expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
		})

		test('data = bigint', () => {
			const data = 9007199254740991n
			expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
		})

		test('data = symbol', () => {
			const data = Symbol('data')
			expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
		})

		test('data = function', () => {
			const data = () => {}
			expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
		})
	})

	describe('typeof(data) == object', () => {
		test('data = null', () => {
			const data = null
			expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
		})

		test('data = list', () => {
			const data = []
			expect(() => UnPivot.convert(data)).toThrow('Assert failed: data is not an object')
		})
	})
})

describe('UnPivot.convert(data) - assert data.dimensions', () => {
	test('data.dimensions = undefined', () => {
		const data = { measures: [] }
		expect(() => UnPivot.convert(data)).toThrow('Assert failed: data.dimensions is requreid')
	})

	test('data.dimensions != list', () => {
		const data = { dimensions: null, measures: [] }
		expect(() => UnPivot.convert(data)).toThrow('Assert failed: data.dimensions is not a list')
	})
})

describe('UnPivot.convert(data) - assert data.dimensions.item', () => {
	test('data.dimensions.item = null', () => {
		const data = { dimensions: [null], measures: [] }
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.dimensions.item is not an object'
		)
	})

	test('data.dimensions.item = list', () => {
		const data = { dimensions: [[]], measures: [] }
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.dimensions.item is not an object'
		)
	})

	test('data.dimensions.item - name is missing', () => {
		const data = { dimensions: [{}], measures: [] }
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.dimensions.item.name is requreid'
		)
	})

	test('data.dimensions.item - type is invalid', () => {
		const data = {
			dimensions: [{ name: 'test', type: 'measure' }],
			measures: []
		}
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.dimensions.item.type is invalid'
		)
	})

	test('data.dimensions.item - values is missing', () => {
		const data = { dimensions: [{ name: 'test' }], measures: [] }
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.dimensions.item.values is requreid'
		)
	})

	test('data.dimensions.item - values is invalid', () => {
		const data = { dimensions: [{ name: 'test', values: null }], measures: [] }
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.dimensions.item.values is not a list'
		)
	})

	test('data.dimensions.item - values length is zero', () => {
		const data = { dimensions: [{ name: 'test', values: [] }], measures: [] }
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.dimensions.item.values length is zero'
		)
	})
})

describe('UnPivot.convert(data) - assert data.measures', () => {
	test('data.measures = undefined', () => {
		const data = { dimensions: [] }
		expect(() => UnPivot.convert(data)).toThrow('Assert failed: data.measures is requreid')
	})

	test('data.measures = null', () => {
		const data = {
			dimensions: [{ name: 'test', values: ['test'] }],
			measures: null
		}
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.measures is not a list or an object'
		)
	})
})

describe('UnPivot.convert(data) - assert data.measures.item', () => {
	test('data.measures.item = null', () => {
		const data = {
			dimensions: [{ name: 'test', values: ['test'] }],
			measures: [null]
		}
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.measures.item is not an object'
		)
	})

	test('data.measures.item = list', () => {
		const data = {
			dimensions: [{ name: 'test', values: ['test'] }],
			measures: [[]]
		}
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.measures.item is not an object'
		)
	})

	test('data.measures.item - name is missing', () => {
		const data = {
			dimensions: [{ name: 'test', values: ['test'] }],
			measures: [{}]
		}
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.measures.item.name is requreid'
		)
	})

	test('data.measures.item - values is missing', () => {
		const data = {
			dimensions: [{ name: 'test', values: ['test'] }],
			measures: [{ name: 'test' }]
		}
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.measures.item.values is requreid'
		)
	})

	test('data.measures.item - values is invalid', () => {
		const data = {
			dimensions: [{ name: 'test', values: ['test'] }],
			measures: [{ name: 'test', values: null }]
		}
		expect(() => UnPivot.convert(data)).toThrow(
			'Assert failed: data.measures.item.values is not a list'
		)
	})

	test('data.measures.item - values dimensions are not the same - 1', () => {
		const data = {
			dimensions: [{ name: 'test', values: ['test'] }],
			measures: [{ name: 'test', values: [] }]
		}
		expect(() => UnPivot.convert(data)).toThrow('Assert failed: dimensions are not the same')
	})

	test('data.measures.item - values dimensions are not the same - 2', () => {
		const data = {
			dimensions: [{ name: 'test', values: ['test'] }],
			measures: [{ name: 'test', values: [[1]] }]
		}
		expect(() => UnPivot.convert(data)).toThrow('Assert failed: dimensions are not the same')
	})
})

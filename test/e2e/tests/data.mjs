const DataCollection = {
	simpleDataWithOneValue: {
		series: [
			{
				name: 'Foo',
				type: 'dimension',
				values: ['Baz']
			},
			{
				name: 'Bar',
				type: 'measure',
				values: [42]
			}
		]
	},
	simpleDataWithTwoValue: {
		series: [
			{
				name: 'Foo',
				type: 'dimension',
				values: ['Baz1', 'Baz2']
			},
			{
				name: 'Bar',
				type: 'measure',
				values: [42, 43]
			}
		]
	},
	simpleDataWithThreeValue: {
		series: [
			{
				name: 'Foo',
				values: [42, 43, 44]
			},
			{
				name: 'Bar',
				values: [2, 3, 4]
			}
		]
	}
}

export default DataCollection

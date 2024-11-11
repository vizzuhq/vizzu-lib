const testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series: [
					{
						name: 'Foo',
						values: ['Alice', 'Bob', 'Ted', 'Alice', 'Bob', 'Ted', 'Alice']
					},
					{ name: 'Foo2', values: ['A', 'A', 'A', 'B', 'B', 'B', 'A'] },
					{ name: 'Foo3', values: ['X', 'Y', 'Z', 'X', 'Y', 'Z', 'Y'] },
					{ name: 'Bar', values: [15, 32, 12, 23, 41, 31, 1] }
				]
			}
		}),
	(chart) =>
		chart.animate({
			x: ['Foo', 'Foo2'],
			y: 'Bar',
			color: 'Foo'
		}),
	(chart) =>
		chart.animate({
			sort: 'byValue'
		}),
	(chart) =>
		chart.animate({
			lightness: 'Bar',
			y: ['Foo3', 'Bar']
		}),
	(chart) =>
		chart.animate({
			sort: 'none'
		})
]

export default testSteps

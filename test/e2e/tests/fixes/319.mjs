const testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series: [
					{
						name: 'Foo',
						values: ['Ted', 'Alice', 'Bob', 'Ted', 'Alice', 'Bob', 'Ted']
					},
					{ name: 'Foo2', values: ['A', 'A', 'A', 'B', 'B', 'B', 'A'] },
					{ name: 'Foo3', values: ['X', 'Y', 'Z', 'X', 'Y', 'Z', 'Y'] },
					{ name: 'Bar', values: [23, 32, 12, 15, 41, 31, 1] }
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
			x: { sort: 'byValue' }
		}),
	(chart) =>
		chart.animate({
			lightness: 'Bar',
			y: ['Foo3', 'Bar']
		}),
	(chart) =>
		chart.animate({
			x: { set: ['Foo2', 'Foo'], labelLevel: 1, sort: 'byLabel' },
			y: { reverse: true }
		})
]

export default testSteps

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
					{ name: 'Foo3', values: ['Y', 'X', 'Z', 'Y', 'X', 'Z', 'X'] },
					{ name: 'Bar', values: [23, 32, 12, 15, 41, 31, 1] }
				]
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				x: ['Foo', 'Foo2', 'Foo3'],
				y: 'Bar',
				color: ['Foo', 'Foo2'],
				legend: 'color'
			}
		}),
	(chart) =>
		chart.animate({
			color: { labelLevel: 1 }
		}),
	(chart) =>
		chart.animate({
			color: { labelLevel: '[0, 1]' }
		}),
	(chart) =>
		chart.animate({
			color: { labelLevel: '[1, 0]' }
		})
]

export default testSteps

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
				y: ['Foo', 'Foo2'],
				x: 'Bar'
			},
		}),
	(chart) =>
		chart.animate({
			y: { labelLevel: '[0, 1]' }
		}),
	(chart) =>
		chart.animate({
			y: { labelLevel: '[1, 0]' }
		}),
	(chart) =>
		chart.animate({
			y: { labelLevel: 1 }
		}),
	(chart) =>
		chart.animate({
			config: {	y: { labelLevel: '[0, 1]' } },
			style: { plot: { yAxis: { label: { multiLevelAxis: 'nested' }}}}
		}),
	(chart) =>
		chart.animate({
			y: ['Foo2', 'Foo']
		}),
	(chart) =>
		chart.animate({
			config: { coordSystem: 'polar' }
		}),
	(chart) =>
		chart.animate({
			config: { y: ['Foo', 'Foo2'] }
		}),
	(chart) =>
		chart.animate({
			config: { y: ['Foo', 'Foo2', 'Foo3'], coordSystem: 'cartesian' }
		}),
	(chart) =>
		chart.animate({
			config: { y: { sort: 'byLabel' } }
		}),
	(chart) =>
		chart.animate({
			config: { y: { labelLevel: '[0, 1, 2]' } },
			style: { plot: { paddingLeft: '9.2em' }}
		}),
	(chart) =>
		chart.animate({
			config: { y: { labelLevel: '[0, 2]' }},
			style: { plot: { paddingLeft: null }}
		})
]

export default testSteps

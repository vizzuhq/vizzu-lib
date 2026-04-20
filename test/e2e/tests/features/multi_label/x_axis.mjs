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
				y: 'Bar'
			}
		}),
	(chart) =>
		chart.animate({
			x: { labelLevel: '[0, 1]', sort: 'byLabel' }
		}),
	(chart) =>
		chart.animate({
			x: { set: ['Foo', 'Foo2'], labelLevel: '[1, 0]', sort: 'none' }
		}),
	(chart) =>
		chart.animate({
			x: { labelLevel: 1 }
		}),
	(chart) =>
		chart.animate({
			config: { x: { labelLevel: '[0, 1]' } },
			style: { plot: { xAxis: { label: { multiLevelAxis: 'nested' } } } }
		}),
	(chart) =>
		chart.animate({
			x: ['Foo2', 'Foo']
		}),
	(chart) =>
		chart.animate({
			config: { coordSystem: 'polar' }
		}),
	(chart) =>
		chart.animate({
			config: { x: ['Foo', 'Foo2'] }
		}),
	(chart) =>
		chart.animate({
			config: { x: ['Foo', 'Foo2', 'Foo3'], coordSystem: 'cartesian' }
		}),
	(chart) =>
		chart.animate({
			config: { x: { sort: 'byLabel' } }
		}),
	(chart) =>
		chart.animate({
			config: { x: { labelLevel: '[0, 1, 2]' } },
			style: { plot: { paddingBottom: '7.25em' } }
		}),
	(chart) =>
		chart.animate({
			config: { x: { labelLevel: '[0, 2]' } },
			style: { plot: { paddingBottom: null } }
		})
]

export default testSteps

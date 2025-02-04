const testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series: [
					{ name: 'Foo', values: ['Alice', 'Bob', 'Ted', 'Jonas'] },
					{ name: 'Bar', values: [15, 32, 12, 55] }
				]
			}
		}),
	(chart) =>
		chart.animate({
			channels: {
				x: { set: 'Foo', reverse: true },
				y: 'Bar'
			},
			geometry: 'area'
		}),

	(chart) =>
		chart.animate({
			coordSystem: 'polar'
		}),
	(chart) =>
		chart.animate({
			x: { sort: 'byValue' },
			coordSystem: 'cartesian',
			geometry: 'line'
		}),
	(chart) =>
		chart.animate({
			x: { reverse: false }
		})
]

export default testSteps

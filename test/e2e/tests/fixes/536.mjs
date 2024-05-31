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
				x: 'Foo',
				y: 'Bar'
			},
			geometry: 'area',
			reverse: true
		}),

	(chart) =>
		chart.animate({
			coordSystem: 'polar'
		}),
	(chart) =>
		chart.animate({
			coordSystem: 'cartesian',
			geometry: 'line',
			sort: 'byValue'
		}),
	(chart) =>
		chart.animate({
			reverse: false
		})
]

export default testSteps

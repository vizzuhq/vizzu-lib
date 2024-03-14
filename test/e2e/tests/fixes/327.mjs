const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted', 'Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: ['A', 'A', 'A', 'B', 'B', 'B'] },
				{ name: 'Baz', values: [1, 2, 3, 6, 5, 4] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: 'Foo',
			y: 'Bar',
			size: 'Baz',
			geometry: 'circle'
		}),
	(chart) =>
		chart.animate({
			geometry: 'line',
			orientation: 'vertical'
		})
]

export default testSteps

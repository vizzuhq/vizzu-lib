const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', '"\\␡\t\n'] },
				{ name: 'Bar', values: [15, 32, 12] },
				{ name: 'Baz', values: [5, 3, 2] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: 'Foo',
			y: 'Bar'
		}),
	(chart) => {
		console.info(chart.data)
		return chart
	}
]

export default testSteps

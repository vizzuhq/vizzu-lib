const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Baz', values: [3, 5, 2] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: 'Foo',
			y: 'Baz'
		}),
	(chart) =>
		chart.animate({
			x: { step: 2 }
		}),
	(chart) =>
		chart.animate({
			x: { step: 3 }
		})
]

export default testSteps

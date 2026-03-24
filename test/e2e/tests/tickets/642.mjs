const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['A', 'B', 'C', 'A', 'B', 'C'] },
				{ name: 'Foo2', values: ['1', '1', '1', '2', '2', '2'] },
				{ name: 'Bar', values: [1, 2, 1, 2, 1, 2] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate(
			{
				x: ['Foo', 'Foo2'],
				y: 'Bar',
				coordSystem: 'polar'
			},
			0
		)
]

export default testSteps

const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Baz', values: [3, 2, 1] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate(
			{
				x: { set: 'Foo', range: { max: 2.5 } },
				y: 'Baz',
				coordSystem: 'polar'
			},
			0
		)
]

export default testSteps

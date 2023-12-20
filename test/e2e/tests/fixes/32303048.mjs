const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [15, 32, 12] }
			]
		}
		return chart.animate({ data }, 0)
	},
	(chart) => {
		return chart.animate(
			{
				data: { filter: (record) => record.Foo === 'Alice' },
				config: {
					x: 'Foo',
					y: ['Foo', 'Bar']
				}
			},
			0
		)
	}
]

export default testSteps

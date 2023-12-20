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
	(chart) =>
		chart.animate(
			{
				config: {
					x: 'Foo',
					y: ['Foo', 'Bar']
				}
			},
			1
		),
	(chart) => chart.animate({}, 2),
	(chart) =>
		chart.animate(
			{
				config: {
					x: ['Foo', 'Bar'],
					y: 'Foo'
				}
			},
			1
		)
]

export default testSteps

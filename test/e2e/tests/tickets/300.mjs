const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [15, 32, 12] },
				{ name: 'Baz', values: [5, 3, 2] }
			]
		}

		return chart.animate({ data })
	},
	(chart) => chart.animate(
		{
			title: 'Foo'
		}
	),
	(chart) => chart.animate({
		config: {
			x: 'Foo',
			y: 'Bar'
		},
		style: {
			paddingLeft: '200px'
		}
	})
]

export default testSteps

const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [15, 4, 12] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate(
			{
				y: 'Bar'
			},
			1
		),
	(chart) =>
		chart.animate({
			config: {
				x: 'Foo',
				color: 'Foo'
			},
			style: {
				plot: {
					marker: {
						colorPalette: '#9355e8FF #123456FF #BDAF10FF'
					}
				}
			}
		})
]

export default testSteps

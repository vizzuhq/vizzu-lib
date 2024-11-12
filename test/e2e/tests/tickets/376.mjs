const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [15, 0.1, 12] }
			]
		}

		return chart.animate({ data })
	},
	(chart) => chart.animate(
		{config : {
				size: 'Bar',
				color: 'Foo',
				geometry: 'circle'
			},
			style: {
				plot: {
					marker: {
						circleMinRadius: 0.05
					}
				}
			}
		}
	)
]

export default testSteps

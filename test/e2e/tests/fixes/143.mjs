const testSteps = [
	(chart) => {
		const data = {
			series: [
				{
					name: 'Colors',
					type: 'dimension',
					values: ['red', 'green', 'blue', 'red', 'green', 'blue', 'red', 'green', 'blue']
				},
				{
					name: 'Letters',
					type: 'dimension',
					values: ['a', 'a', 'a', 'b', 'b', 'b', 'c', 'c', 'c']
				},
				{
					name: 'Val',
					type: 'measure',
					values: [3, 5, 4, 3 + 1, 5 + 1, 4 + 1, 3 + 2, 5 + 2, 4 + 2]
				}
			]
		}

		return chart.animate({
			data,
			config: {
				y: 'Colors',
				x: 'Val'
			}
		})
	},
	(chart) =>
		chart.animate(
			{
				y: 'Val',
				x: 'Letters',
				split: true
			},
			{ regroupStrategy: 'drilldown' }
		),
	(chart) =>
		chart.animate(
			{
				y: ['Colors', 'Val']
			}
		)
]

export default testSteps

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
				y: { set: 'Val', split: true },
				x: { set: 'Letters', split: true }
			},
			{ regroupStrategy: 'drilldown' }
		)
]

export default testSteps

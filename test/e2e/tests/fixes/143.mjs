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
					name: 'Letters2',
					type: 'dimension',
					values: ['a', 'a', 'a', 'a', 'a', 'b', 'b', 'b', 'b']
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
				x: 'Val',
				split: true
			}
		})
	},
	(chart) =>
		chart.animate(
			{
				y: ['Letters2', 'Val'],
				x: 'Letters'
			},
			{ regroupStrategy: 'drilldown' }
		),
	(chart) =>
		chart.animate(
			{
				y: ['Letters2', 'Colors', 'Val']
			},
			{ regroupStrategy: 'drilldown' }
		),
	(chart) =>
		chart.animate({
			config: {
				y: 'Colors',
				x: 'Val'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				y: ['Letters2', 'Val'],
				x: 'Letters'
			}
		}),
	(chart) =>
		chart.animate({
			y: ['Letters2', 'Val']
		}),
	(chart) =>
		chart.animate({
			y: ['Letters2', 'Colors', 'Val']
		}),
	(chart) =>
		chart.animate({
			config: {
				y: ['Letters2', 'Colors'],
				x: 'Val'
			}
		})
]

export default testSteps

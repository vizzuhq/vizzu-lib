const testSteps = [
	(chart) => {
		const data = {
			series: [
				{
					name: 'Colors',
					type: 'dimension',
					values: [
						'red', 'green', 'blue',
						'red', 'green', 'blue',
						'red', 'green', 'blue',
					]
				},
				{
					name: 'Letters',
					type: 'dimension',
					values: [
						'a', 'a', 'a',
						'b', 'b', 'b',
						'c', 'c', 'c',
					]
				},
				{
					name: 'Val',
					type: 'measure',
					values: [ 3, -5, 4,
						-4, 6, 5,
						-5, 7, 6 ,
					]
				}
			]
		}

		return chart.animate({ data })
	},
	(chart) => chart.animate(
		{
			y: ['Colors','Val'],
			x: 'Letters',
			split: true
		}
	)
]

export default testSteps

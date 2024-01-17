const series = [
	{
		name: 'Y',
		type: 'measure',
		values: [1, 2]
	},
	{
		name: 'X',
		type: 'dimension',
		values: ['|', '|']
	}
]

const testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series
			}
		})
]

for (let i = 95; i <= 105; i++) {
	series.push({
		name: 'X' + i,
		type: 'dimension',
		values: ['|'.repeat(i), '|'.repeat(i + 1)]
	})
	testSteps.push((chart) =>
		chart.animate(
			{
				x: ['X' + i, 'X'],
				y: 'Y',
				title: 'Count of |: ' + i
			},
			{ regroupStrategy: 'drilldown', duration: 0.5 }
		)
	)
}

export default testSteps

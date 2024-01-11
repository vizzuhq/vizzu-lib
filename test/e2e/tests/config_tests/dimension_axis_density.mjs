let testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series: [
					{
						name: 'C',
						type: 'dimension',
						values: ['1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12']
					},
					{
						name: 'Values 2',
						type: 'measure',
						values: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12]
					}
				]
			}
		}),
	(chart) =>
		chart.animate(
			{
				x: 'C',
				y: 'Values 2'
			},
			0
		)
]

for (let i = 11; i <= 66; i++) {
	testSteps.push((chart) =>
		chart.animate(
			{
				x: {
					step: i / 11,
					title: 'Step: ' + i / 11
				}
			},
			0.3
		)
	)
}

for (let i = 7; i <= 12; i++) {
	testSteps.push((chart) =>
		chart.animate(
			{
				x: {
					step: i,
					title: 'Step: ' + i
				}
			},
			0.3
		)
	)
}

export default testSteps

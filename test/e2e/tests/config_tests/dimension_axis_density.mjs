const testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series: [
					{
						name: 'C',
						type: 'dimension',
						values: [
							'1',
							'2',
							'3',
							'3,5',
							'4',
							'5',
							'6',
							'7',
							'8',
							'9',
							'10',
							'11',
							'12'
						]
					},
					{
						name: 'Values 2',
						type: 'measure',
						values: [1, 2, 3, 3.5, 4, 5, 6, 7, 8, 9, 10, 11, 12]
					}
				],
				filter: (record) => record.C !== '3,5'
			}
		}),
	(chart) =>
		chart.animate(
			{
				x: { set: 'C', title: 'Step: 1' },
				y: 'Values 2',
				color: 'C',
				legend: 'color'
			},
			0
		)
]

for (let i = 1; i <= 5; i++) {
	for (let j = 12 / i - 1; j >= 1; j--) {
		const num = (i * j + 1) / (j - 0.0000001)
		testSteps.push((chart) =>
			chart.animate(
				{
					x: {
						step: num,
						title: 'Step: ' + Math.round(num * 1000) / 1000
					}
				},
				0.3
			)
		)
	}
}

for (let i = 6; i <= 12; i++) {
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

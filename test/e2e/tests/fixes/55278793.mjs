const testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series: [
					{
						name: 'D1',
						values: ['v1', 'v2']
					},
					{
						name: 'M1',
						values: [-1, -2]
					}
				]
			},
			config: {
				size: ['D1', 'M1']
			}
		})
]

export default testSteps

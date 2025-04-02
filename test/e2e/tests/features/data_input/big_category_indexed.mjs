const data = {
	series: [
		{
			name: 'Dim1',
			categories: Array.from(new Array(10000).keys()).map((a) => a + ''),
			values: [1, 1, 3, 3, 5, 5, 7, 7]
		},
		{
			name: 'Dim2',
			categories: Array.from(new Array(10000).keys()).map((a) => a + ''),
			values: [0, 1, 0, 1, 0, 1, 0, 1],
			type: 'dimension'
		},
		{
			name: 'Meas',
			type: 'measure',
			values: [1, 2, 3, 4, 5, 6, 7, 8]
		}
	]
}

const testSteps = [
	(chart) => chart.animate({ data }),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => record.Dim1 !== '5'
			},
			config: {
				x: ['Dim1', 'Dim2'],
				y: 'Meas',
				color: 'Dim2',
				split: true
			}
		}),
	(chart) =>
		chart.animate(
			{
				data: {
					filter: () => true
				},
				config: {
					x: 'Dim2',
					y: ['Dim1', 'Meas'],
					color: 'Dim2'
				}
			},
			{
				show: { delay: 0 }
			}
		)
]

export default testSteps

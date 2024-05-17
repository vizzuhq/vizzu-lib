const data = {
	series: [
		{
			name: 'Dim1',
			categories: ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'],
			values: [1, 1, 3, 3, 5, 5, 7, 7]
		},
		{
			name: 'Dim2',
			categories: ['0', '1'],
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
			x: ['Dim1', 'Dim2'],
			y: 'Meas',
			color: 'Dim2'
		})
]

export default testSteps

const data = {
	series: [
		{
			name: 'Dim0',
			type: 'dimension',
			values: ['a', 'a', 'a', 'a', 'b', 'b', 'b']
		},
		{
			name: 'Dim1',
			type: 'dimension',
			values: ['A', 'B', 'C', 'B', 'A', 'A', 'B']
		},
		{
			name: 'Meas',
			type: 'measure',
			values: [1, 2, 3, 4, 5, 6, 7]
		}
	]
}

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				x: 'min(Meas)',
				y: 'max(Meas)',
				label: 'mean(Meas)',
				size: 'sum(Meas)',
				color: 'Dim0',
				geometry: 'circle',
				legend: 'size'
			}
		})
]

export default testSteps

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
	(chart) => chart.animate({ data }),
	(chart) =>
		chart.animate({
			x: 'Dim0',
			y: 'Meas',
			label: 'Meas',
			title: 'default: 10, 18'
		}),
	(chart) =>
		chart.animate({
			x: 'Dim0',
			y: 'min(Meas)',
			label: 'min(Meas)',
			title: 'min(): 1, 5'
		}),
	(chart) =>
		chart.animate({
			x: 'Dim0',
			y: 'max(Meas)',
			label: 'max(Meas)',
			title: 'max(): 4, 7'
		}),
	(chart) =>
		chart.animate({
			x: 'Dim0',
			y: 'mean(Meas)',
			label: 'mean(Meas)',
			title: 'mean(): 2.5, 6'
		}),
	(chart) =>
		chart.animate({
			x: 'Dim0',
			y: 'sum(Meas)',
			label: 'sum(Meas)',
			title: 'sum(): 10, 18'
		}),
	(chart) =>
		chart.animate({
			x: 'Dim0',
			y: 'count()',
			label: 'count()',
			title: 'count(): 4, 3'
		}),
	(chart) =>
		chart.animate({
			x: 'Dim0',
			y: 'distinct(Dim1)',
			label: 'distinct(Dim1)',
			title: 'distinct(): 3, 2'
		}),
	(chart) => chart.animate({ x: 'Dim0', y: 'Meas', label: 'Meas', title: 'default' })
]

export default testSteps

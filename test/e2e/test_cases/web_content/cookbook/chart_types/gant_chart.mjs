const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Task', type: 'dimension' },
				{ name: 'Helper', type: 'dimension' },
				{ name: 'Days', type: 'measure' }
			],
			records: [
				['Task 1', 'start', 0],
				['Task 1', 'length', 1],
				['Task 2', 'start', 1],
				['Task 2', 'length', 2],
				['Task 3', 'start', 3],
				['Task 3', 'length', 3],
				['Task 4', 'start', 3],
				['Task 4', 'length', 7],
				['Task 5', 'start', 6],
				['Task 5', 'length', 5]
			]
		}

		return chart.animate({
			data,
			config: {
				x: ['Days', 'Helper'],
				y: 'Task',
				color: 'Helper',
				title: 'Gant Chart',
				legend: null,
				reverse: true
			},
			style: {
				'plot.marker.colorPalette': '#ffffff00 #01abf6ff'
			}
		})
	}
]

export default testSteps

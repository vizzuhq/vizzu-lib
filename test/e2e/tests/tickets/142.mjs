const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'X', values: ['A', 'A', 'A', 'A', 'B', 'B', 'B', 'B'] },
				{ name: 'Y0', values: ['A', 'A', 'B', 'B', 'A', 'A', 'B', 'B'] },
				{ name: 'Y1', values: ['A', 'B', 'A', 'B', 'A', 'B', 'A', 'B'] },
				{ name: 'm', values: [4, 3, 2, 1, 10, 6, 7, -8] },
				{ name: 'm0', values: [1, 2, 1, 3, 2, 1, 4, 3] },
				{ name: 'm1', values: [2, 2.5, 3, 1, 1, 3, 2, 1.5] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			y: ['Y0', 'm'],
			x: ['X', 'm0'],
			color: { set: 'Y0', title: 'Y0' },
			label: ['X', 'm']
		}),
	(chart) =>
		chart.animate({
			y: ['Y1', 'm'],
			x: ['X', 'm1'],
			color: { set: 'Y1', title: 'Y1' }
		})
]

export default testSteps

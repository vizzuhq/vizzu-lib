const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'X', values: ['A', 'B', 'C', 'D'] },
				{ name: 'm', values: [3, 1, 2, 4] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			config: {
				y: 'm',
				x: { set: 'X', range: { min: 1, max: 3 } },
				geometry: 'line'
			},
			style: {
				paddingRight: 100,
				paddingLeft: 100
			}
		}),
	(chart) =>
		chart.animate({
			x: { range: { min: 0.99, max: 3.01 } }
		})
]

export default testSteps

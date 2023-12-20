const data = {
	series: [
		{ name: 'Dim0', type: 'dimension', values: ['a', 'b'] },
		{ name: 'Meas0', type: 'measure', values: [-0.5, 1] },
		{ name: 'Meas1', type: 'measure', values: [-0.3, 1] }
	]
}

const testSteps = [
	(chart) => chart.animate({ data }),
	(chart) =>
		chart.animate({
			x: 'Meas0',
			y: 'Meas1',
			label: 'Dim0',
			title: 'Hello World',
			subtitle: 'As a starting point',
			caption: 'Source: Vizzu tutorial'
		}),
	(chart) =>
		chart.animate({
			title: null,
			subtitle: null,
			caption: 'Vizzu tutorial'
		}),
	(chart) =>
		chart.animate({
			subtitle: "I'm back",
			caption: null
		})
]

export default testSteps

const data = {
	series: [
		{ name: 'Year', type: 'dimension', values: [] },
		{ name: 'Round2', type: 'dimension', values: [] },
		{ name: 'Won', type: 'measure', values: [] }
	],
	records: [
		[2005, 'First round', 1],
		[2005, 'Second round', 1],
		[2005, 'Third round', 1],
		[2005, 'Fourth round', 0]
	]
}

const testSteps = [
	(chart) => chart.animate({ data, config: { x: 'Year', y: 'Round2' } }),

	(chart) =>
		chart.animate({
			config: { lightness: 'Won' },
			style: { plot: { marker: { maxLightness: 0, minLightness: 0.8 } } }
		})
]

export default testSteps

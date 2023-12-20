const testSteps = [
	async (chart) => {
		await import('https://d3js.org/d3.v7.min.js')

		const data = { series: [], records: [] }

		data.records = await d3.csv(
			'https://raw.githubusercontent.com/vizzuhq/vizzu-lib-doc/3dac11c1ff8076f5bf2801aa13e56b01120a3c61/docs/datasets/population_total_long.csv'
		)

		data.series = Object.keys(data.records[0]).map((name) => ({
			name,
			type: name === 'Count' ? 'measure' : 'dimension'
		}))

		for (let i = 0; i < data.records.length; i++)
			data.records[i] = Object.values(data.records[i])

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			config: {
				x: 'Year',
				y: 'Count',
				geometry: 'line'
			},
			style: {
				'plot.xAxis.label': {
					angle: '-45deg'
				}
			}
		})
]

export default testSteps

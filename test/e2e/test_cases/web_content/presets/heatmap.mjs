import { data_6 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.heatmap({
				x: 'Year',
				y: 'Country_code',
				lightness: 'Value 3 (+)',
				title: 'Heatmap'
			}),
			style: {
				plot: {
					marker: {
						rectangleSpacing: 0
					}
				}
			}
		})
]

export default testSteps

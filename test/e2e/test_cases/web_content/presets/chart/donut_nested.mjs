import { data_3 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_3,
			config: chart.constructor.presets.nestedDonut({
				angle: 'Value 2 (+)',
				stackedBy: 'Joy factors',
				radius: 'Country',
				title: 'Nested Donut Chart'
			}),
			style: {
				plot: {
					marker: {
						rectangleSpacing: '0',
						borderWidth: 1,
						borderOpacity: 0
					}
				}
			}
		})
]

export default testSteps

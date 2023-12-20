import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					y: 'Value 2 (+)',
					label: 'Value 2 (+)'
				},
				title: 'Column'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Country'
				},
				title: 'DD & Group for Categories of New Main Dimension'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

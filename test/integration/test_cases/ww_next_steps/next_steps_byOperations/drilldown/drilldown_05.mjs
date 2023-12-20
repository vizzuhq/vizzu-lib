import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					y: ['Country', 'Value 2 (+)'],
					color: 'Country',
					label: 'Value 2 (+)'
				},
				title: 'Stacked Column'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					label: null
				},
				title: 'DD & Group Existed Categories by New Main Dimension'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

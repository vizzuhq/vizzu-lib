import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Country',
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
					x: null,
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				},
				title: 'Stacked Column to Sum of the Values'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

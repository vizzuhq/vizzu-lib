import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				},
				title: 'Groupped Column'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					label: 'Value 2 (+)'
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

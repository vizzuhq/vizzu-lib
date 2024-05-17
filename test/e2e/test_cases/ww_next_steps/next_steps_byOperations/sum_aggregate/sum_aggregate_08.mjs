import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					x: ['Year', 'Country'],
					y: 'Value 2 (+)',
					color: 'Country',
					label: null
				},
				title: 'Groupped Column'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: null,
					label: 'Value 2 (+)'
				},
				title: 'Stacked Column to Sum Bar of the Values'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

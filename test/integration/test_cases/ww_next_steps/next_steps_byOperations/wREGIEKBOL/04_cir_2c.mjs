import { data_14 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,

			config: {
				channels: {
					y: 'Value 4 (+/-)',
					x: 'Value 2 (+)',
					color: 'Country',
					label: 'Country'
				},
				title: 'Scatter plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 4 (+/-)',
					x: 'Value 2 (+)',
					color: 'Country',
					noop: 'Year',
					label: null
				},
				title: 'Drill Down to See the Distribution of Detailed Values'
			}
		})
]

export default testSteps

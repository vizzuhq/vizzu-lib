import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				},
				title: 'Stacked Column'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 5 (+/-)',
					y: 'Value 2 (+)',
					noop: 'Year',
					label: 'Year'
				},
				title: 'Change Geom. to See Distribution by a New Measure',
				geometry: 'circle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

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
					x: 'Value 4 (+/-)',
					color: 'Country',
					label: 'Country'
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

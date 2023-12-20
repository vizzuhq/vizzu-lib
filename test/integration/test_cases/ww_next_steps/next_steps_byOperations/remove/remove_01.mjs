import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Value 4 (+/-)',
					y: 'Value 2 (+)',
					color: 'Country',
					label: 'Value 4 (+/-)'
				},
				title: 'Scatter plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Country',
					color: null,
					label: 'Value 2 (+)'
				},
				title: 'If Remove a Measure, then a Column Chart is Better',
				geometry: 'rectangle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

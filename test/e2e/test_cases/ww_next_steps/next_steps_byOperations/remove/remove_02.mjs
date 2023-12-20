import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Value 5 (+/-)',
					y: 'Value 2 (+)',
					color: 'Country',
					noop: 'Year'
				},
				title: 'Scatter plot',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country',
					noop: null
				},
				title: 'If Remove a Measure, then a Stacked Column is Better',
				geometry: 'rectangle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

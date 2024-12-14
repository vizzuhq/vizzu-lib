import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: { set: ['Value 1 (+)', 'Country'], split: true },
					y: 'Value 3 (+)',
					noop: 'Year',
					color: 'Country'
				},
				title: 'Trellis Scatter plot',
				geometry: 'circle',
				orientation: 'vertical'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 1 (+)'
				},
				title: 'Split off Components to Compare Values'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

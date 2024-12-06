import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					noop: 'Year',
					size: null,
					color: 'Country'
				},
				title: 'Scatter plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Value 1 (+)', 'Country'], split: true }
				},
				title: 'Split Components to See them Separately',
				geometry: 'circle',
				orientation: 'vertical'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

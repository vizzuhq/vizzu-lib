import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					size: 'Value 2 (+)',
					noop: 'Year',
					label: ['Year', 'Value 2 (+)']
				},
				title: 'Bubble plot',
				geometry: 'circle',
				legend: 'size'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: null,
					label: 'Year'
				},
				title: 'If Remove a Measure, then a Scatter plot is Better',
				legend: 'noop'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

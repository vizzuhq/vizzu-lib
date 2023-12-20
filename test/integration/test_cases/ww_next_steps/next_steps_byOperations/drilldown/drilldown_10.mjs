import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					noop: 'Joy factors',
					label: 'Joy factors'
				},
				title: 'Scatter plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					color: 'Country',
					label: null
				},
				title: 'Drill Down, then Unstack new Values'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

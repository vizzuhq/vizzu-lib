import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					noop: 'Country',
					size: 'Value 2 (+)',
					label: 'Value 2 (+)'
				},
				title: 'Bubble',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					color: 'Year',
					label: null
				},
				title: 'Drill Down & Add +1 Measure to See Distribution by New Values'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

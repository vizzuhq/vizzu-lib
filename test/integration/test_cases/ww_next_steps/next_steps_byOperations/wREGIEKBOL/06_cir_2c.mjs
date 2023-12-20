import { data_14 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,

			config: {
				channels: {
					y: 'Value 6 (+/-)',
					x: 'Value 2 (+)',
					color: 'Joy factors',
					noop: 'Joy factors',
					size: null
				},
				title: 'Scatter plot',
				geometry: 'circle'
			},
			style: {
				plot: {
					marker: {
						colorPalette: '#ef675aFF #6d8cccFF #e6cf99FF #9c50abFF'
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 6 (+/-)',
					x: 'Value 2 (+)',
					color: 'Joy factors',
					noop: 'Year'
				},
				title: 'Drill Down to See the Distribution of Detailed Values'
			}
		})
]

export default testSteps

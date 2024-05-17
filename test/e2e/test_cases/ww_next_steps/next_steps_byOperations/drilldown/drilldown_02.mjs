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
					y: ['Value 2 (+)', 'Joy factors'],
					color: 'Joy factors'
				},
				title: 'Drill Down for Components of New 2nd Dimension'
			},
			style: {
				plot: {
					marker: {
						colorPalette: '#ef675aFF #6d8cccFF #e6cf99FF #9c50abFF'
					}
				}
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

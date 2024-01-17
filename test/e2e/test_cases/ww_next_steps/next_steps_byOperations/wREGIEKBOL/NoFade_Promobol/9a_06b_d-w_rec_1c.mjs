import { data_4 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					color: 'Country',
					size: ['Year', 'Value 2 (+)'],
					label: 'Value 2 (+)'
				},
				title: 'Bubble chart',
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
					size: null,
					label: null
				},
				title: 'Then Show the Sum of The Values by Time Distribution',
				geometry: 'rectangle'
			},
			style: {
				plot: {
					marker: {
						colorPalette: null
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

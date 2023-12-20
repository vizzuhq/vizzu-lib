import { data_3 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_3,

			config: {
				channels: {
					x: null,
					y: null,
					color: 'Country',
					size: ['Year', 'Value 2 (+)'],
					label: 'Value 2 (+)'
				},
				title: 'Treemap'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country',
					label: null
				},
				title: 'Then Show the Sum of The Values by Time Distribution',
				geometry: 'rectangle'
			}
		})
]

export default testSteps

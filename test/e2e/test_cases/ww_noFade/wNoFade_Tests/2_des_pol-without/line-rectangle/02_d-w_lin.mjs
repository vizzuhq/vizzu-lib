import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)',
					color: 'Country',
					size: 'Value 1 (+)'
				},
				title: 'Line Chart',
				geometry: 'line'
			},
			style: {
				plot: {
					marker: { lineMaxWidth: 0.02 }
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					noop: 'Year',
					lightness: 'Value 3 (+)',
					size: ['Year', 'Value 1 (+)']
				},
				title: 'Change Geoms & CoordSys',
				geometry: 'rectangle'
			}
		})
]

export default testSteps

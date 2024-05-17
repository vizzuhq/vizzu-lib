import { data_3 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_3,

			config: {
				channels: {
					x: 'Year',
					y: 'Value 1 (+)',
					color: 'Country'
				},
				title: 'Line Chart',
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					size: ['Year', 'Value 1 (+)'],
					label: 'Value 1 (+)'
				},
				title: 'FORDITVA: CHG Geom. & Coord.Sys. to See Values in Time',
				geometry: 'circle'
			}
		})
]

export default testSteps

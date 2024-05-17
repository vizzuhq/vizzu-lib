import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country',
					size: 'Year'
				},
				title: 'Stacked Area Chart',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					color: 'Country',
					size: ['Year', 'Value 2 (+)'],
					label: 'Value 2 (+)'
				},
				title: 'FORDITVA: CHG Geom. & Coord.Sys. to See Values in Time',
				geometry: 'circle'
			}
		})
]

export default testSteps

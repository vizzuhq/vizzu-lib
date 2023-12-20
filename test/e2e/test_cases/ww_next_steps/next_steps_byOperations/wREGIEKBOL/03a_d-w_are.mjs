import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				},
				title: 'Stacked Area',
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
					noop: 'Year',
					size: ['Year', 'Value 3 (+)']
				},
				title: 'FORDITVA: CHG Geom. & Coord.Sys. to See Values in Time',
				geometry: 'rectangle'
			}
		})
]

export default testSteps

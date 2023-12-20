import { data_14 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,

			config: {
				channels: {
					y: null,
					noop: ['Year', 'Country'],
					size: 'Value 2 (+)'
				},
				title: 'Bubble Chart',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					noop: ['Year', 'Country'],
					y: 'Value 2 (+)',
					size: null
				},
				title: 'Change Coord. Sys. to See Distribution of Values'
			}
		})
]

export default testSteps

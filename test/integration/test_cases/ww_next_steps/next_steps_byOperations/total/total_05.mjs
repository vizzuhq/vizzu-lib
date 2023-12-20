import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					noop: 'Year',
					size: 'Value 2 (+)',
					color: 'Country'
				},
				title: 'Bubble plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					noop: null,
					color: null,
					label: 'Value 2 (+)'
				},
				title: 'Remove Categ. & CHG Coord. Sys. to Aggregate of Components',
				legend: null,
				geometry: 'circle'
			},
			style: {
				plot: {
					marker: {
						label: {
							fontSize: '1.5em'
						}
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

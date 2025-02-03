import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Country', 'Value 2 (+)'], split: true },
					color: 'Country',
					label: 'Value 2 (+)'
				},
				title: 'Splitted Column'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							fontSize: '0.6em'
						}
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Year', 'Country'],
					y: { set: 'Value 2 (+)', split: false },
					label: null
				},
				title: 'Split off & Group Markers to Compare Values'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

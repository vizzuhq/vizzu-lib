import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

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
					x: 'Value 5 (+/-)',
					y: { set: 'Value 2 (+)', split: false },
					noop: 'Year',
					label: null
				},
				title: 'Change Geom. to See Distribution by a New Measure',
				geometry: 'circle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

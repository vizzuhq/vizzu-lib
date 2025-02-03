import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					x: ['Year', 'Country'],
					y: 'Value 2 (+)',
					color: 'Country'
				},
				title: 'Groupped Column',
				geometry: 'rectangle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: { set: ['Country', 'Value 2 (+)'], split: true },
					label: 'Value 2 (+)'
				},
				title: 'Split Components to See them Separately'
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
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

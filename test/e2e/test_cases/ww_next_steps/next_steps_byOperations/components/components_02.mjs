import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: ['Country', 'Year'],
					y: 'Value 2 (+)',
					color: 'Country'
				},
				title: 'Groupped Column by Countries and Time'
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
				title: 'Split Components to Compare Components Year by Year'
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

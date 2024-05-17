import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,
			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: ['Joy factors', 'Value 3 (+)'],
					color: 'Joy factors',
					label: ['Country', 'Value 2 (+)']
				},
				title: 'Marimekko Chart',
				align: 'stretch',
				orientation: 'horizontal'
			},
			style: {
				plot: {
					marker: {
						label: {
							format: 'dimensionsFirst',
							fontSize: '0.7em'
						}
					}
				}
			}
		})
]

export default testSteps

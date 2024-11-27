import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,
			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: { set: ['Joy factors', 'Value 3 (+)'], align: 'stretch' },
					color: 'Joy factors',
					label: ['Value 2 (+)']
				},
				title: 'Marimekko Chart',
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

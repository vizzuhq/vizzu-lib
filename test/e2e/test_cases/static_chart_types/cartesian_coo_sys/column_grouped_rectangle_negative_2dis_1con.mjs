import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: 'Value 5 (+/-)',
					x: ['Joy factors', 'Country'],
					color: 'Joy factors',
					label: 'Value 5 (+/-)'
				},
				title: 'Grouped Column Chart with (-) Nums'
			},
			style: {
				plot: {
					marker: {
						label: {
							fontSize: 7,
							orientation: 'vertical',
							angle: 3.14 * -1
						}
					}
				}
			}
		})
]

export default testSteps

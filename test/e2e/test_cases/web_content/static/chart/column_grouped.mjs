import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Joy factors', 'Country'],
					y: 'Value 5 (+/-)',
					color: 'Joy factors',
					label: 'Value 5 (+/-)'
				},
				title: 'Grouped Column Chart'
			},
			// Labels have to be rotated on this chart.
			style: {
				plot: {
					marker: {
						label: {
							fontSize: 6,
							orientation: 'vertical',
							angle: 3.14 * -1
						}
					}
				}
			}
		})
]

export default testSteps

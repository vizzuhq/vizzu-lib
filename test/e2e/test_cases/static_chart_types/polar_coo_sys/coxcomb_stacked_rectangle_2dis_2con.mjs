import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: ['Joy factors', 'Value 2 (+)'],
					x: ['Year', 'Value 2 (+)'],
					color: 'Joy factors'
				},
				title: 'Stacked Coxcomb Chart',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					marker: {
						borderOpacity: 1
					}
				}
			}
		})
]

export default testSteps

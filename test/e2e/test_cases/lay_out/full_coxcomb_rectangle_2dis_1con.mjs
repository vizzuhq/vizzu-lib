import { data } from '../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: {
						attach: ['Joy factors', 'Value 2 (+)'],
						range: { min: '0%', max: '107.5%' }
					},
					x: { attach: ['Year'] },
					color: { attach: ['Joy factors'] }
				},
				title: 'Stacked Coxcomb Chart',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					backgroundColor: 'rgb(215,215,215 )',
					marker: {
						rectangleSpacing: 0.1
					}
				}
			}
		})
]

export default testSteps

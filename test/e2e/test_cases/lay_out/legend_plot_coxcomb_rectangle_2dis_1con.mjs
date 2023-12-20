import { data } from '../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: {
						attach: ['Joy factors', 'Value 2 (+)'],
						range: { min: '0%', max: '1.075%' }
					},
					x: { attach: ['Year'] },
					color: { attach: ['Joy factors'] }
				},
				title: null,
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

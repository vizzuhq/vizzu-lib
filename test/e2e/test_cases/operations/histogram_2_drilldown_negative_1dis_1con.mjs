import { data } from '../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Year'] },
					y: { attach: ['Value 2 (+)'], range: { min: '0%', max: '110%' } },
					label: { attach: ['Value 2 (+)'] }
				},
				title: 'Histogram with (-) Nums'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Joy factors'] },
					color: { attach: ['Joy factors'] }
				},
				title: 'Stacked',
				legend: 'color'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: {
						detach: ['Joy factors', 'Value 2 (+)'],
						attach: ['Value 5 (+/-)'],
						range: { min: '0%', max: '110%' }
					},
					x: { attach: ['Joy factors'] },
					label: { detach: ['Value 2 (+)'], attach: ['Value 5 (+/-)'] }
				},
				title: 'Grouped with (-) Nums'
			},
			style: {
				plot: {
					marker: {
						label: {
							fontSize: 8,
							position: 'top',
							orientation: 'vertical',
							angle: -1 * 3.14
						}
					}
				}
			}
		})
]

export default testSteps

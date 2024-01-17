import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
					x: { attach: ['Value 6 (+/-)'], range: { min: '0%', max: '110%' } },
					color: { attach: ['Country'] },
					size: { attach: ['Value 4 (+/-)'] },
					label: { attach: ['Value 5 (+/-)'] }
				},
				title: 'Color Discrete series Changing Anim.',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: {
						attach: ['Year']
					},
					label: {
						detach: ['Value 5 (+/-)']
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: {
						detach: ['Year'],
						attach: ['Country']
					},
					color: {
						detach: ['Country'],
						attach: ['Year']
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: {
						detach: ['Country']
					},
					label: {
						attach: ['Year']
					}
				}
			}
		})
]

export default testSteps

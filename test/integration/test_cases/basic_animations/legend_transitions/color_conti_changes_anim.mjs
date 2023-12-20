import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
					x: { attach: ['Value 6 (+/-)'], range: { min: '0%', max: '110%' } },
					color: { attach: ['Value 4 (+/-)'] },
					size: { attach: ['Value 4 (+/-)'] },
					label: { attach: ['Country'] }
				},
				title: 'Color Continuous series Changing Anim.',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					color: { detach: ['Value 4 (+/-)'], attach: ['Value 5 (+/-)'] }
				}
			}
		})
]

export default testSteps

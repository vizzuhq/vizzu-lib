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
					size: { attach: ['Value 1 (+)'] },
					label: { attach: ['Value 1 (+)'] }
				},
				title: 'Color legend On-Off Anim.',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					color: { detach: ['Country'] }
				}
			}
		})
]

export default testSteps

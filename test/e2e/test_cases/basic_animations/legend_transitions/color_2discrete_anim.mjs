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
				title: 'Color 2 Discrete series.',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					color: { attach: ['Joy factors'] },
					size: { attach: ['Joy factors'] },
					label: { detach: ['Value 5 (+/-)'] }
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { detach: ['Joy factors'] },
					label: { attach: ['Value 5 (+/-)'] }
				}
			}
		})
]

export default testSteps

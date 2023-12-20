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
				title: 'Color legend On-Off Anim.',
				legend: 'color',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				legend: null
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				legend: 'color'
			}
		})
]

export default testSteps

import { data } from '../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '150%' } },
					x: { attach: ['Value 6 (+/-)'], range: { min: '0%', max: '150%' } },
					color: { attach: ['Value 5 (+/-)'] },
					size: { attach: ['Value 4 (+/-)'] },
					label: { attach: ['Value 5 (+/-)'] },
					noop: { attach: ['Country'] }
				},
				title: 'Default Color Gradient',
				legend: 'color',
				geometry: 'circle'
			}
		})
]

export default testSteps

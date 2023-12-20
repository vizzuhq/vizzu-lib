import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { attach: ['Value 5 (+/-)'], range: { min: '0%', max: '110%' } },
					x: { attach: ['Value 6 (+/-)'], range: { min: '0%', max: '110%' } },
					lightness: { attach: ['Country'] },
					color: { attach: ['Value 5 (+/-)'] },
					size: { attach: ['Value 4 (+/-)'] },
					label: { attach: ['Country'] }
				},
				title: 'Color Continuous Anim.',
				legend: 'color',
				geometry: 'circle'
			},
			style: {
				plot: {
					marker: {
						colorGradient:
							'#3d51b8 0, #6389ec 0.15, #9fbffa 0.35, #d5d7d9 0.5, #f4b096 0.65, #e36c56 0.85, #ac1727 1'
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record.Country !== 'Germany' &&
					record.Country !== 'Italy' &&
					record.Country !== 'Hungary' &&
					record.Country !== 'Netherlands'
			},
			config: {
				title: 'Color Continuous Anim. - Filtered'
			}
		})
]

export default testSteps

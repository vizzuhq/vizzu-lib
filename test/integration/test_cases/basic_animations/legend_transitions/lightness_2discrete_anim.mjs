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
					size: { attach: ['Value 4 (+/-)'] },
					label: { attach: ['Value 5 (+/-)'] }
				},
				title: 'Lightness 2 Discrete series.',
				legend: 'lightness',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					lightness: { attach: ['Joy factors'] }
				}
			}
		})
]

export default testSteps

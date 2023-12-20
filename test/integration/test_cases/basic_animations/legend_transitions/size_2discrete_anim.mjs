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
					size: { attach: ['Country_code', 'Value 4 (+/-)'] },
					label: { attach: ['Year'] }
				},
				title: 'Size 1 Discrete series.',
				legend: 'size',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					label: { detach: ['Year'] },
					noop: { attach: ['Year'] }
				},
				title: 'Size 1 Discrete series..'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { attach: ['Year'] },
					label: { attach: ['Year'] }
				},
				title: 'Size 2 Discrete series (stacked).'
			}
		})
]

export default testSteps

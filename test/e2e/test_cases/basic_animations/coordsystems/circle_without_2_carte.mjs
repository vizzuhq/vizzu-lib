import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: { attach: ['Joy factors'] },
					size: { attach: ['Value 2 (+)'] },
					label: { attach: ['Country_code'] }
				},
				title: 'Without 2 Cartesian - Bubble 2 Scatter.',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { attach: ['Value 2 (+)'] },
					y: { attach: ['Value 5 (+/-)'] },
					size: { detach: ['Value 2 (+)'], attach: ['Value 1 (+)'] }
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { detach: ['Value 2 (+)'] },
					y: { detach: ['Value 5 (+/-)'] },
					size: { detach: ['Value 1 (+)'], attach: ['Value 2 (+)', 'Country_code'] }
				}
			}
		})
]

export default testSteps

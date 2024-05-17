import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: { attach: ['Joy factors'] },
					size: { attach: ['Value 2 (+)', 'Country_code'] },
					label: { attach: ['Country_code'] }
				},
				title: 'Without 2 Cartesian - Treemap 2 Column.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { attach: ['Joy factors'] },
					y: { attach: ['Value 2 (+)', 'Country_code'] },
					size: { detach: ['Value 2 (+)'] },
					lightness: { attach: ['Country_code'] }
				}
			},
			style: {
				plot: {
					marker: {
						label: { fontSize: 6 }
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { attach: ['index'] },
					label: { detach: ['Country_code'] }
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { detach: ['index', 'Joy factors'] },
					y: { detach: ['Value 2 (+)', 'Country_code'] },
					label: { attach: ['Country_code'] },
					size: { attach: ['Value 2 (+)'] }
				}
			},
			style: {
				plot: {
					marker: {
						label: { fontSize: null }
					}
				}
			}
		})
]

export default testSteps

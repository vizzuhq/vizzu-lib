import { data } from '../../test_data/chart_precision.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Values child'], range: { min: '0%', max: '110%' } },
					y: { attach: ['Parents'] },
					label: { attach: ['Values child', 'Childs'] },
					size: { attach: ['Values child'] }
				},
				title: 'Chart Precision Line (-) - X',
				geometry: 'line'
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top'
						}
					}
				}
			}
		})
]

export default testSteps

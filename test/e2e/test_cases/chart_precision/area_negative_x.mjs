import { data } from '../../test_data/chart_precision.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: {
						attach: ['Childs', 'Values child'],
						range: { min: '0%', max: '110%' }
					},
					y: { attach: ['Parents'] },
					label: { attach: ['Values child'] },
					lightness: { attach: ['Childs'] }
				},
				title: 'Chart Precision Area - X',
				geometry: 'area'
			}
		})
]

export default testSteps

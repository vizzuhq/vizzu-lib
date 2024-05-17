import { data } from '../../test_data/chart_precision.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Parents'] },
					y: { attach: ['Values child'], range: { min: '0%', max: '110%' } },
					label: { attach: ['Childs', 'Values child'] },
					size: { attach: ['Values child'] }
				},
				title: 'Chart Precision Line (-) - Y',
				geometry: 'line'
			}
		})
]

export default testSteps

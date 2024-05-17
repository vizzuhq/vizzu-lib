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
					y: { attach: ['Parents', 'Childs'] },
					label: { attach: ['Values child'] }
				},
				title: 'Chart Precision Rectangle (-) - X'
			}
		})
]

export default testSteps

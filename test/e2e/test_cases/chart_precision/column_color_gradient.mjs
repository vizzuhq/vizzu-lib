import { data } from '../../test_data/chart_precision.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Parents', 'Childs'] },
					y: {
						attach: ['Parents', 'Childs', 'Values child'],
						range: { min: '110%', max: '0%' }
					},
					color: { attach: ['Values child'] }
				},
				title: 'Waterfall (-) Color Gradient',
				legend: 'color'
			}
		})
]

export default testSteps

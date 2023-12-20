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
					color: { attach: ['Parents'] },
					lightness: { attach: ['Values child'] },
					label: { attach: ['Values child'] }
				},
				title: 'Waterfall (-) lightness',
				legend: 'lightness'
			}
		})
]

export default testSteps

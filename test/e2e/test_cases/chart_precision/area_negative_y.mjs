import { data } from '../../test_data/chart_precision.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Parents'] },
					y: {
						attach: ['Childs', 'Values child'],
						range: { min: '0%', max: '110%' }
					},
					label: { attach: ['Values child'] },
					lightness: { attach: ['Childs'] }
				},
				title: 'Chart Precision Area',
				geometry: 'area'
			}
		})
]

export default testSteps

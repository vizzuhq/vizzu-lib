import { data } from '../../test_data/chart_precision.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Parents'] },
					y: {
						attach: ['Childs', 'Values parent'],
						range: { min: '0%', max: '110%' }
					},
					label: { attach: ['Values parent'] }
				},
				title: 'Chart Precision Rectangle - Y'
			}
		})
]

export default testSteps

import { data } from '../../test_data/chart_precision.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: {
						attach: ['Childs', 'Values parent'],
						range: { min: '0%', max: '110%' }
					},
					y: { attach: ['Parents'] },
					label: { attach: ['Values parent'] },
					size: { attach: ['Values parent'] }
				},
				title: 'Chart Precision Line - X',
				geometry: 'line'
			}
		})
]

export default testSteps

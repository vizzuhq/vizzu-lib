import { data } from '../../test_data/chart_precision.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					label: { attach: ['Values parent'] },
					size: { attach: ['Values parent', 'Parents'] },
					color: { attach: ['Childs'] }
				},
				title: 'Bubble',
				geometry: 'circle'
			}
		})
]

export default testSteps

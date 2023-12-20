import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					label: { attach: ['Values 1'] },
					color: { attach: ['Categ. Parent'] },
					size: { attach: ['Values 1'] }
				},
				title: 'Treemap - Aggregated.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { attach: ['Categ. Child'] }
				},
				title: 'Treemap - Drill Down.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { detach: ['Categ. Child'] }
				},
				title: 'Treemap - Aggregated.'
			}
		})
]

export default testSteps

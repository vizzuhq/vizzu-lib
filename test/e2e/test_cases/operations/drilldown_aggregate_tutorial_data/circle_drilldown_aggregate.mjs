import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Values 1'] },
					y: { attach: ['Values 2'], range: { min: '0%', max: '110%' } },
					color: { attach: ['Categ. Parent'] },
					size: { attach: ['Values 3'] },
					label: { attach: ['Categ. Parent'] }
				},
				title: '1. Operations: Circle - Aggregated.',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						size: { attach: ['Categ. Child'] }
					},
					title: '2. Operations: Circle - Drill Down.'
				}
			},
			{ duration: 0.3 }
		),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { detach: ['Categ. Child'] },
					noop: { attach: ['Categ. Child'] }
				},
				title: '3. Operations: Circle - Drill Down.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					noop: { detach: ['Categ. Child'] },
					size: { attach: ['Categ. Child'] }
				},
				title: '4. Operations: Circle - Aggregated.'
			}
		}),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						size: { detach: ['Categ. Child'] }
					},
					title: '5. Operations: Circle - Aggregated.'
				}
			},
			{ duration: 0.3 }
		)
]

export default testSteps

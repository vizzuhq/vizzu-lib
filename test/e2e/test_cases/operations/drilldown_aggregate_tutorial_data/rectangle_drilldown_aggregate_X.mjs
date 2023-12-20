import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Timeseries'] },
					y: { attach: ['Values 1'], range: { min: '0%', max: '110%' } },
					label: { attach: ['Values 1'] }
				},
				title: 'Operations: Rectangle - Aggregated - X axis.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Categ. Parent'] },
					color: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Rectangle - Drill Down - X axis.',
				legend: 'color'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Categ. Parent'] },
					x: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Rectangle - Drill Down - X axis.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Categ. Parent'] },
					x: { detach: ['Categ. Parent'] }
				},
				title: 'Operations: Rectangle - Aggregated - X axis.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Categ. Parent'] },
					color: { detach: ['Categ. Parent'] }
				},
				title: 'Operations: Rectangle - Aggregated - X axis.',
				legend: null
			}
		})
]

export default testSteps

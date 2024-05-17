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
				title: 'Operations: Line - Aggregated - Y axis.',
				geometry: 'line'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Line - Drill Down - Y axis.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { detach: ['Categ. Parent'] },
					color: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Line - Drill Down - Y axis.',
				legend: 'color'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					color: { detach: ['Categ. Parent'] },
					size: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Line - Aggregated - Y axis.',
				legend: null
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { detach: ['Categ. Parent'] }
				},
				title: 'Operations: Line - Aggregated - Y axis.'
			}
		})
]

export default testSteps

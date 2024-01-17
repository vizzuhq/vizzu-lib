import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Timeseries', 'Categ. Parent'] },
					y: { attach: ['Values 1'], range: { min: '0%', max: '110%' } },
					label: { attach: ['Values 1'] }
				},
				title: 'Operations: Line - Drilled Down - X axis.',
				geometry: 'line'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { detach: ['Categ. Parent'] },
					size: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Line - Aggregated - X axis.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { detach: ['Categ. Parent'] }
				},
				title: 'Operations: Line - Drill Down - X axis.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Line - Aggregated - X axis.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { attach: ['Categ. Parent'] },
					size: { detach: ['Categ. Parent'] }
				},
				title: 'Operations: Line - Aggregated - X axis.'
			}
		})
]

export default testSteps

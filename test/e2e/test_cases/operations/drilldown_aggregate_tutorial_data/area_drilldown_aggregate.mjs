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
				title: 'Operations: Area - Aggregated.',
				geometry: 'area',
				legend: null
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Categ. Parent'] },
					color: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Area - Drill Down.',
				legend: 'color'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Categ. Parent'] },
					color: { detach: ['Categ. Parent'] }
				},
				title: 'Operations: Area - Aggregated.',
				legend: null
			}
		})
]

export default testSteps

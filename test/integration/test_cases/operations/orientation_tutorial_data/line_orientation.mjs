import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Timeseries'] },
					y: { attach: ['Values 1'], range: { min: '0%', max: '110%' } },
					color: { attach: ['Categ. Parent'] },
					label: { attach: ['Values 1'] }
				},
				title: 'Operations: Line - Orientation Change - No:1.',
				geometry: 'line',
				orientation: 'horizontal'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Values 1'], range: { min: 'auto', max: 'auto' } },
					x: { attach: ['Values 1'] },
					color: { attach: ['Timeseries'] }
				},
				title: 'Operations: Line - Orientation Change - No:2',
				orientation: 'vertical'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { detach: ['Values 1'] },
					y: { attach: ['Values 1'] },
					color: { attach: ['Categ. Parent'] }
				},
				orientation: 'horizontal'
			}
		})
]

export default testSteps

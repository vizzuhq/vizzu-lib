import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Timeseries'] },
					y: {
						attach: ['Values 1', 'Categ. Parent'],
						range: { min: '0%', max: '110%' }
					},
					color: { attach: ['Categ. Parent'] },
					label: { attach: ['Values 1'] }
				},
				title: 'Operations: Area - Stacked.',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Categ. Parent'] }
				},
				title: 'Operations: Area - Groupped.',
				geometry: 'line'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Area - Stacked.',
				geometry: 'area'
			}
		})
]

export default testSteps

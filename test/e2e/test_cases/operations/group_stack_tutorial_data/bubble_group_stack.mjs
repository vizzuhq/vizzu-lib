import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					size: { attach: ['Values 1', 'Timeseries', 'Categ. Parent'] },
					color: { attach: ['Categ. Parent'] },
					label: { attach: ['Values 1'] }
				},
				title: 'Operations: Circle - Groupped.',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { detach: ['Categ. Parent'] },
					noop: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Circle - Stacked.'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: { attach: ['Categ. Parent'] },
					noop: { detach: ['Categ. Parent'] }
				},
				title: 'Operations: Circle - Groupped.'
			}
		})
]

export default testSteps

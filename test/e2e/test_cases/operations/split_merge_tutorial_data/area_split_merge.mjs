import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Timeseries'] },
					y: {
						attach: ['Categ. Parent', 'Values 1'],
						range: { min: '0%', max: '110%' }
					},
					color: { attach: ['Categ. Parent'] },
					label: { attach: ['Values 1'] }
				},
				title: 'Operations: Area - Merged.',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Operations: Area - Splitted.',
				split: true
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Operations: Area - Merged.',
				split: false
			}
		})
]

export default testSteps

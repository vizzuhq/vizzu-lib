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
				title: 'Operations: Rectangle - Merged.',
				geometry: 'rectangle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Operations: Rectangle - Splitted.',
				split: true
			},
			style: { plot: { marker: { label: { position: 'top' } } } }
		}),
	(chart) =>
		chart.animate({
			config: {
				title: 'Operations: Rectangle - Merged.',
				split: false
			},
			style: { plot: { marker: { label: { position: null } } } }
		})
]

export default testSteps

import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Values 2'] },
					y: { attach: ['Values 1'], range: { min: '0%', max: '110%' } },
					color: { attach: ['Categ. Parent'] },
					size: { attach: ['Values 3'] },
					label: { attach: ['Categ. Child', 'Values 1'] }
				},
				title: 'Operations: Circle - Merged.',
				geometry: 'circle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { attach: ['Categ. Parent'] }
				},
				title: 'Operations: Circle - Splitted.',
				orientation: 'horizontal',
				split: true,
				align: 'none'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { detach: ['Categ. Parent', 'Categ. Child'] }
				},
				title: 'Operations: Circle - Merged.',
				split: false
			}
		})
]

export default testSteps

import { data } from '../../../test_data/tutorial.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Timeseries'] },
					y: { attach: ['Values 1', 'Categ. Parent'] },
					color: {
						attach: ['Categ. Parent'],
						range: { min: '0%', max: '110%' }
					},
					label: { attach: ['Values 1'] }
				},
				title: 'Operations: Area - Filter.',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record['Categ. Parent'] !== 'A' && record['Categ. Parent'] !== 'B'
			},
			config: {
				title: 'Operations: Area - Filtered.'
			}
		})
]

export default testSteps

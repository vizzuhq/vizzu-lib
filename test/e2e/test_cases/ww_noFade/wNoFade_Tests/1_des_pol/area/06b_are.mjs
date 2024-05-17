import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: 'Country' }
				},
				title: 'Stacked Area Chart',
				geometry: 'area',
				orientation: 'horizontal'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: ['Joy factors', 'Value 3 (+)'] },
					color: { set: 'Joy factors' }
				},
				title: 'Change Conti & Add Color'
			}
		})
]

export default testSteps

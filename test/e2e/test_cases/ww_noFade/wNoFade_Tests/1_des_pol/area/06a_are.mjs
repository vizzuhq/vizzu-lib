import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: 'Country_code' },
					y: { set: ['Joy factors', 'Value 2 (+)'] },
					color: { set: 'Joy factors' }
				},
				title: 'Stacked Area Chart',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: ['Joy factors', 'Value 3 (+)'] }
				},
				title: 'Add Conti'
			}
		})
]

export default testSteps

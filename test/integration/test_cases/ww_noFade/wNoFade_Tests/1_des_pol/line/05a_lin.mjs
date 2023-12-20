import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: 'Country_code' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Joy factors' },
					size: { set: 'Value 1 (+)' }
				},
				title: 'Line Chart',
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 3 (+)' },
					color: { set: 'Joy factors' },
					size: { set: 'Value 1 (+)' }
				},
				title: 'Group new Disc & Add new Conti'
			}
		})
]

export default testSteps

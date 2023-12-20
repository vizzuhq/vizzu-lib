import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Joy factors' }
				},
				title: 'Line Chart',
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: 'Joy factors' }
				},
				title: 'Stack Discrete & Remove Conti'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Country_code'] }
				},
				title: 'Change Discrete'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Country_code' },
					y: { set: 'Value 1 (+)' },
					color: { set: 'Joy factors' },
					size: null
				},
				title: 'Group new Disc, Add new Conti'
			}
		})
]

export default testSteps

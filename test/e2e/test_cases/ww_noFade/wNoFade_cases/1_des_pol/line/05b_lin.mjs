import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Country' },
					size: { set: 'Value 1 (+)' }
				},
				title: 'Line Chart',
				geometry: 'line'
			},
			style: {
				plot: {
					marker: { lineMaxWidth: 0.02 }
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					color: { set: null },
					size: { set: ['Country', 'Value 1 (+)'] }
				},
				title: 'Stack Disc & Remove Color'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					color: { set: null },
					size: { set: ['Country', 'Value 3 (+)'] }
				},
				title: 'Change Conti'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						size: { set: ['Joy factors', 'Value 3 (+)'] }
					},
					title: 'Change Discrete'
				}
			},
			{
				duration: 0
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					color: { set: 'Joy factors' },
					size: { set: 'Value 1 (+)' }
				},
				title: 'Group new Disk & Add new Color'
			}
		})
]

export default testSteps

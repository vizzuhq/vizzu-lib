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
					x: { set: 'Year' },
					y: { set: ['Joy factors'] },
					color: { set: 'Joy factors' }
				},
				title: 'Stack Disc & Remove Conti'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Country_code' },
						y: { set: ['Joy factors'] },
						color: { set: 'Joy factors' }
					},
					title: 'Change Disc'
				}
			},
			{
				duration: 0.5
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Country_code' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Joy factors' }
				},
				title: 'Group new Disc & Add previous Conti'
			}
		})
]

export default testSteps

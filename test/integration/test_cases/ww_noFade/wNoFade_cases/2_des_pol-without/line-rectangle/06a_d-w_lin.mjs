import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Country' }
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
					y: { set: 'Country' },
					color: { set: 'Country' }
				},
				title: 'Stack Disc & Remove Conti'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Joy factors' },
						y: { set: 'Country' },
						color: { set: 'Country' }
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
					x: { set: 'Joy factors' },
					y: { set: 'Value 3 (+)' },
					color: { set: 'Country' }
				},
				title: 'Group new Disc & Add new Conti'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: null,
						y: null,
						color: { set: 'Country' },
						size: { set: ['Joy factors', 'Value 3 (+)'] }
					},
					title: 'Stack new Disc & Change Geoms & CoordSys',
					geometry: 'rectangle'
				}
			},
			{
				geometry: {
					delay: 0.5,
					duration: 1.5
				},
				x: {
					delay: 1
				}
			}
		)
]

export default testSteps

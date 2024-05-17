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
					y: { set: 'Value 2 (+)' },
					color: null,
					size: { set: 'Country' }
				},
				title: 'Stack Disc & Remove Color'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 1 (+)' },
					color: null,
					size: { set: 'Country' }
				},
				title: 'Change Conti'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'Value 1 (+)' },
						color: null,
						size: { set: 'Joy factors' }
					},
					title: 'Change Disc'
				}
			},
			{
				duration: 0
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: null,
						y: null,
						color: { set: 'Joy factors' },
						size: { set: ['Year', 'Value 1 (+)'] }
					},
					title: 'Add new Disc Color & Change Geoms & CoordSys',
					geometry: 'circle'
				}
			},
			{
				geometry: {
					duration: 1
				},
				x: {
					delay: 0.5
				}
			}
		)
]

export default testSteps

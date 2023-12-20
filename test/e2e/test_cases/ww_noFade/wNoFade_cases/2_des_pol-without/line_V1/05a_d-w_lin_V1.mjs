import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Joy factors' },
					size: { set: 'Value 3 (+)' }
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
					color: { set: 'Joy factors' },
					size: { set: 'Value 3 (+)' }
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
						y: { set: 'Joy factors' },
						color: { set: 'Joy factors' },
						size: { set: 'Value 3 (+)' }
					},
					title: 'Change Disc'
				}
			},
			{
				duration: 0.5
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
						size: { set: ['Country_code', 'Value 1 (+)'] },
						lightness: { set: 'Value 3 (+)' }
					},
					title: 'Add new Conti & Change Geoms & CoordSys',
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

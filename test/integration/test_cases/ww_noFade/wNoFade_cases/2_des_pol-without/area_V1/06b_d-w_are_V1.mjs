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
					y: { set: ['Joy factors', 'Value 2 (+)'] },
					color: null
				},
				title: 'Change Disc & Remove Color'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: ['Joy factors', 'Value 3 (+)'] },
					color: null
				},
				title: 'Change Conti'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: null,
						y: null,
						color: { set: 'Joy factors' },
						noop: { set: 'Year' },
						size: { set: ['Year', 'Value 3 (+)'] }
					},
					title: 'Change Geoms & Change CoordSys',
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

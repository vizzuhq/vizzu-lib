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
		chart.animate(
			{
				config: {
					channels: {
						x: null,
						y: null,
						noop: { set: 'Year' },
						lightness: { set: 'Value 3 (+)' },
						size: { set: ['Year', 'Value 1 (+)'] }
					},
					title: 'Change Conti & Geoms & CoordSys',
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

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
					x: { set: null },
					y: { set: null },
					noop: { set: 'Year' },
					lightness: { set: 'Value 3 (+)' },
					size: { set: ['Year', 'Value 1 (+)'] }
				},
				title: 'Change Geoms & CoordSys',
				geometry: 'circle'
			}
		})
]

export default testSteps

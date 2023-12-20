import { data_14 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)',
					color: 'Joy factors'
				},
				title: 'Line Chart',
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					color: 'Joy factors',
					size: ['Country_code', 'Value 2 (+)']
				},
				title: 'Stack new Disc & Change Geoms & CoordSys',
				geometry: 'rectangle'
			}
		})
]

export default testSteps

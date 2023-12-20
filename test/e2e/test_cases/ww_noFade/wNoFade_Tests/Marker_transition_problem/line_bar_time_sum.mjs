import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: 'Value 2 (+)',
					x: 'Year',
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
					y: 'Joy factors',
					x: 'Value 2 (+)',
					label: 'Value 2 (+)'
				},
				geometry: 'rectangle'
			}
		})
]

export default testSteps

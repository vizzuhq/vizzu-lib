import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: 'Value 5 (+/-)',
					color: 'Country'
				},
				title: 'Line Chart',
				geometry: 'line'
			}
		})
]

export default testSteps

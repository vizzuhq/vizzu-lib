import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)',
					color: 'Country'
				},
				title: 'Polar Line Chart',
				geometry: 'line',
				coordSystem: 'polar'
			}
		})
]

export default testSteps

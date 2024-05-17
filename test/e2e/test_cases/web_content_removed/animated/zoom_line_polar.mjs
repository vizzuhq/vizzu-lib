import { data_6 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: {
				channels: {
					x: ['Year', 'Joy factors'],
					y: 'Value 3 (+)',
					color: 'Country_code'
				},
				title: 'Polar Line chart',
				geometry: 'line',
				coordSystem: 'polar'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => data_6.filter(record) && record.Year < 8 && record.Year > 2
			},
			config: {
				title: 'Zoomed Polar Line chart'
			}
		})
]

export default testSteps

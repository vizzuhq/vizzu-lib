import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				},
				title: 'Polar Stacked Area Chart',
				coordSystem: 'polar',
				geometry: 'area',
				legend: null
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Year', 'Value 2 (+)'],
					y: 'Country',
					color: 'Country'
				},
				title: 'Radial Bar Chart',
				geometry: 'rectangle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				},
				title: 'Polar Stacked Area Chart',
				geometry: 'area',
				legend: null
			}
		})
]

export default testSteps

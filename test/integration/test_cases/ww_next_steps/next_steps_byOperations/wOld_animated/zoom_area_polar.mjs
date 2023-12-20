import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,
			config: {
				channels: {
					x: 'Year',
					y: ['Value 2 (+)', 'Country_code'],
					color: 'Country_code'
				},
				title: 'Polar Stacked Area',
				geometry: 'area',
				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			data: {
				filter: (record) => data_8.filter(record) && record.Year < 21 && record.Year > 14
			},
			config: {
				title: 'Zoomed Polar Stacked Area'
			}
		})
]

export default testSteps

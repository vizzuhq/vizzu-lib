import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,
			config: {
				channels: {
					x: ['Year'],
					y: ['Value 2 (+)', 'Country_code'],
					color: 'Country_code'
				},
				title: 'Stacked Area',
				geometry: 'area'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => data_8.filter(record) && record.Year < 21 && record.Year > 14
			},
			config: {
				title: 'Zoomed Stacked Area'
			}
		})
]

export default testSteps

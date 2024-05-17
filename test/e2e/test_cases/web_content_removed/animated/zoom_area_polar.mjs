import { data_6 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: {
				channels: {
					x: ['Year', 'Joy factors'],
					y: ['Value 3 (+)', 'Country_code'],
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
				filter: (record) => data_6.filter(record) && record.Year < 12 && record.Year > 6
			},
			config: {
				title: 'Zoomed Polar Stacked Area'
			}
		})
]

export default testSteps

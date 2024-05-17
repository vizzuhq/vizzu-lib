import { data_6 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: {
				channels: {
					x: ['Value 3 (+)', 'Country'],
					y: ['Year', 'Joy factors'],
					color: 'Country'
				},
				title: 'Stacked Bar Chart'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				title: 'Split Bar Chart',
				split: true
			}
		})
]

export default testSteps

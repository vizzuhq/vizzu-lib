import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: ['Value 2 (+)', 'Country'],
					color: 'Country'
				},
				title: 'Splitted Area',
				geometry: 'area',
				split: true
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				title: 'Split off to Sum of the Components',
				split: false
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

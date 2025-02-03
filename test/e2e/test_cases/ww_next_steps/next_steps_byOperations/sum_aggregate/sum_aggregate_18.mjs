import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Value 2 (+)', 'Country'], split: true },
					color: 'Country'
				},
				title: 'Splitted Area',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				title: 'Split off to Sum of the Components',
				y: { split: false }
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

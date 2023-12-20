import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)'
				},
				title: 'Area',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: ['Value 2 (+)', 'Country'],
					color: 'Country'
				},
				title: 'Drill Down to See New Components'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

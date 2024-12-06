import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Value 2 (+)', 'Country'], align: 'stretch' },
					color: 'Country'
				},
				title: 'Percentage Stacked Area',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: 'Value 2 (+)', align: 'none' }
				},
				title: 'Align min, CHG Geom. & Unstack to Comp. Components',
				geometry: 'line'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

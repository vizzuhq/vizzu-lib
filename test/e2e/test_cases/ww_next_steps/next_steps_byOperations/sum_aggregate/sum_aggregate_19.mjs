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
				title: 'Percentage Stacked Area',
				geometry: 'area',
				align: 'stretch'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				title: 'Align min to See Sum instead of Ratio of Components',
				align: 'none'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Country', 'Value 2 (+)'], align: 'stretch' },
					color: 'Country'
				},
				title: 'Percentage Stacked Column'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				title: 'Align min to Sum of the Values(Components?)',
				y: { align: 'none' }
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

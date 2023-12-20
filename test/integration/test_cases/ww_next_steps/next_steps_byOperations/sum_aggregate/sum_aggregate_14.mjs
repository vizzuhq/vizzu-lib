import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					size: 'Value 2 (+)',
					color: 'Year',
					label: 'Country_code'
				},
				title: 'Bubble plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: 'Value 2 (+)',
					label: 'Year'
				},
				title: 'Remove Category to Aggregate & See Sum of Components'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

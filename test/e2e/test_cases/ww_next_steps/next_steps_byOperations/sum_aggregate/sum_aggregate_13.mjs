import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					color: 'Country',
					label: 'Year'
				},
				title: 'Scatter plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					//               color: null,
					label: 'Country'
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

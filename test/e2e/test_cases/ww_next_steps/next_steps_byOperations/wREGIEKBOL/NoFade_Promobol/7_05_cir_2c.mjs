import { data_4 } from '../../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					y: 'Value 3 (+)',
					x: 'Value 2 (+)',
					color: 'Country'
				},
				title: 'Scatter plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					noop: 'Year'
				},
				title: 'Anim. Aids Drilling for Following New Value Distribution'
			}
		})
]

export default testSteps

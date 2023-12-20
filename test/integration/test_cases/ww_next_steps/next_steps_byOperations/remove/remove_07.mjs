import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: 'Value 3 (+)',
					noop: 'Year',
					color: 'Country'
				},
				title: 'Dot plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Country'
				},
				title: 'Dot plot, Oriented by the Other Category'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

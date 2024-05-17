import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Country',
					noop: ['Year', 'Country']
				},
				title: 'Dot plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 3 (+)',
					noop: 'Year',
					color: 'Country'
				},
				title: "Let's See the Distribution by a New Measure"
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

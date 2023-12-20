import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)',
					color: 'Country'
				},
				title: 'Lollipop',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: ['Country', 'Value 2 (+)'],
					noop: null
				},
				title: 'Change and Stack Geometry to Sum of Components(Values?)',
				geometry: 'rectangle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

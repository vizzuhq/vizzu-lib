import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					noop: 'Year',
					y: 'Value 2 (+)',
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
					x: 'Value 3 (+)'
				},
				title: 'Add +1 Measure'
			}
		})
]

export default testSteps

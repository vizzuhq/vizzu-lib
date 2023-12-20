import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Value 6 (+/-)',
					y: 'Value 5 (+/-)',
					noop: 'Year',
					label: 'Year'
				},
				title: 'Scatter Plot',
				geometry: 'circle'
			}
		})
]

export default testSteps

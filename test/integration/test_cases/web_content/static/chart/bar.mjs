import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Value 5 (+/-)',
					y: 'Country',
					color: 'Country',
					label: 'Value 5 (+/-)'
				},
				title: 'Bar Chart'
			}
		})
]

export default testSteps

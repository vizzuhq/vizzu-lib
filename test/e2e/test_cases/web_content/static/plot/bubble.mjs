import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Value 6 (+/-)',
					y: 'Value 5 (+/-)',
					color: 'Country',
					size: 'Value 4 (+/-)',
					label: 'Value 5 (+/-)'
				},
				title: 'Bubble Plot',
				geometry: 'circle'
			}
		})
]

export default testSteps

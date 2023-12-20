import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Value 4 (+/-)',
					y: 'Value 5 (+/-)',
					noop: 'Country',
					color: 'Joy factors',
					size: 'Value 3 (+)'
				},
				title: 'Bubble Plot',
				geometry: 'circle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

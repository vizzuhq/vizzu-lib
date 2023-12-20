import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: 'Country',
					label: 'Country_code',
					lightness: 'Value 5 (+/-)',
					size: 'Value 5 (+/-)'
				},
				title: 'Bubble Chart',
				geometry: 'circle'
			}
		})
]

export default testSteps

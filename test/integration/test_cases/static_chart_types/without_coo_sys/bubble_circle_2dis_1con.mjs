import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: 'Joy factors',
					size: ['Country_code', 'Value 2 (+)'],
					label: 'Country_code'
				},
				title: 'Bubble chart',
				geometry: 'circle'
			}
		})
]

export default testSteps

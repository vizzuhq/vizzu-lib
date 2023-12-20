import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: 'Joy factors',
					size: 'Value 2 (+)',
					label: 'Country_code'
				},
				title: 'Bubble Chart',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: ['Value 2 (+)', 'Country_code']
				},
				title: 'Stacked Bubble Chart'
			}
		})
]

export default testSteps

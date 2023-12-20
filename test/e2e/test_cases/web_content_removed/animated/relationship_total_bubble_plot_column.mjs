import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Joy factors', 'Value 6 (+/-)'],
					y: 'Value 5 (+/-)',
					color: 'Joy factors',
					size: 'Value 2 (+)',
					label: 'Country_code'
				},
				title: 'Bubble Plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Joy factors',
					x: ['Value 2 (+)', 'Country_code'],
					label: null
				},
				title: 'Bar Chart',
				geometry: 'rectangle',
				orientation: 'vertical'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 2 (+)',
					label: 'Value 2 (+)'
				}
			}
		})
]

export default testSteps

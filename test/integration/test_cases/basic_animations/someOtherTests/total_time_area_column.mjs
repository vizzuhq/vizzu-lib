import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: ['Joy factors', 'Value 2 (+)'],
					x: 'Year',
					color: 'Joy factors'
				},
				title: 'Stacked Area Chart',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				title: 'Split Area Chart',
				split: true
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: ['Value 2 (+)', 'Year'],
					x: 'Joy factors'
				},
				title: 'Column Chart',
				geometry: 'rectangle',
				split: false
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 2 (+)',
					x: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Column Chart',
				geometry: 'rectangle',
				split: false
			}
		})
]

export default testSteps

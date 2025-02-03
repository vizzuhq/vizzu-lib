import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { set: ['Joy factors', 'Value 2 (+)'], split: true },
					x: 'Year',
					color: 'Joy factors'
				},
				title: 'Split Area Chart',
				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: 'Value 2 (+)', split: false },
					x: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Column Chart',
				geometry: 'rectangle'
			}
		})
]

export default testSteps

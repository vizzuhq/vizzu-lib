import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Value 2 (+)', 'Joy factors'],
					y: 'Year',
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Split Bar Chart',
				split: true
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

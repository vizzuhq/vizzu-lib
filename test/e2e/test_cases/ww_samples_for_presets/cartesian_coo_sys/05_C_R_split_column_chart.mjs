import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { set: ['Value 2 (+)', 'Joy factors'], split: true },
					x: 'Year',
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Split Column Chart'
			}
		}),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

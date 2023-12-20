import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Joy factors',
					y: 'Value 2 (+)',
					label: 'Value 2 (+)'
				},
				title: 'Column Chart'
			}
		})
]

export default testSteps

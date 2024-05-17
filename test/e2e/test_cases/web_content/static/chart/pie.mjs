import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors',
					label: 'Value 2 (+)'
				},
				title: 'Pie Chart',
				coordSystem: 'polar'
			}
		})
]

export default testSteps

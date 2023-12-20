import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors'
				},
				title: 'Coxcomb Chart',
				coordSystem: 'polar'
			}
		})
]

export default testSteps

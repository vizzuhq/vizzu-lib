import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.pie({
				angle: 'Value 2 (+)',
				by: 'Joy factors',
				title: 'Pie Chart'
			})
		})
]

export default testSteps

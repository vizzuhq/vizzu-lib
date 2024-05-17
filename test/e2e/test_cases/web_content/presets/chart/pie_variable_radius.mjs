import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.variableRadiusPie({
				angle: 'Value 2 (+)',
				radius: 'Value 1 (+)',
				by: 'Joy factors',
				title: 'Variable Radius Pie Chart'
			})
		})
]

export default testSteps

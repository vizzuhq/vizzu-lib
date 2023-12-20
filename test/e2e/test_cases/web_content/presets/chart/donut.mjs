import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.donut({
				angle: 'Value 2 (+)',
				stackedBy: 'Joy factors',
				title: 'Donut Chart'
			})
		})
]

export default testSteps

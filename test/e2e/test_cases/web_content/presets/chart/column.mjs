import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.column({
				x: 'Joy factors',
				y: 'Value 2 (+)',
				title: 'Column Chart'
			})
		})
]

export default testSteps

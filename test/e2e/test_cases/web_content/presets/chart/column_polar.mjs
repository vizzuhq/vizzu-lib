import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.polarColumn({
				angle: 'Joy factors',
				radius: 'Value 2 (+)',
				title: 'Polar Column Chart'
			})
		})
]

export default testSteps

import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.bubble({
				size: 'Value 5 (+/-)',
				color: 'Country_code',
				title: 'Bubble Chart'
			})
		})
]

export default testSteps

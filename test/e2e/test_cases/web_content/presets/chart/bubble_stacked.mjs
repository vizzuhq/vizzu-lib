import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.stackedBubble({
				size: 'Value 2 (+)',
				color: 'Joy factors',
				stackedBy: 'Country_code',
				title: 'Stacked Bubble Chart'
			})
		})
]

export default testSteps

import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.splittedBar({
				x: 'Value 2 (+)',
				y: 'Year',
				splittedBy: 'Joy factors',
				title: 'Splitted Bar Chart'
			})
		})
]

export default testSteps

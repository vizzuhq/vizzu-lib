import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.splittedColumn({
				x: 'Year',
				y: 'Value 2 (+)',
				splittedBy: 'Joy factors',
				title: 'Splitted Column Chart'
			})
		})
]

export default testSteps

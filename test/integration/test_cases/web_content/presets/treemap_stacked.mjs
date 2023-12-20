import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.stackedTreemap({
				size: 'Value 2 (+)',
				color: 'Joy factors',
				title: 'Stacked Treemap',
				dividedBy: 'Country_code'
			})
		})
]

export default testSteps

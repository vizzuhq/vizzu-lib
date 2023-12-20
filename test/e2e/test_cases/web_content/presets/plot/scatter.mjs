import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.scatter({
				x: 'Value 6 (+/-)',
				y: 'Value 5 (+/-)',
				dividedBy: 'Year',
				title: 'Scatter Plot'
			})
		})
]

export default testSteps

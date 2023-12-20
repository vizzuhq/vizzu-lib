import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.area({
				x: 'Year',
				y: 'Value 5 (+/-)',
				title: 'Area Chart'
			})
		})
]

export default testSteps

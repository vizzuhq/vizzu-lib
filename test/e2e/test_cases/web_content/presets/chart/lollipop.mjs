import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.lollipop({
				x: 'Year',
				y: 'Value 1 (+)',
				title: 'Lollipop Chart'
			})
		})
]

export default testSteps

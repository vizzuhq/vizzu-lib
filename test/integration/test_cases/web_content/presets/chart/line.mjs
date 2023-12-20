import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.line({
				x: 'Year',
				y: 'Value 6 (+/-)',
				dividedBy: 'Country',
				title: 'Line Chart'
			})
		})
]

export default testSteps

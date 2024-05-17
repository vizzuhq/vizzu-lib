import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.line({
				x: 'Value 6 (+/-)',
				y: 'Year',
				dividedBy: 'Country',
				title: 'Vertical Line Chart'
			})
		})
]

export default testSteps

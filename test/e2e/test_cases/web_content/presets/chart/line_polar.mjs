import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.polarLine({
				angle: 'Year',
				radius: 'Value 2 (+)',
				dividedBy: 'Country',
				title: 'Polar Line Chart'
			})
		})
]

export default testSteps

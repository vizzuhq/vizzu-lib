import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.percentageArea({
				x: 'Year',
				y: 'Value 2 (+)',
				stackedBy: 'Country',
				title: 'Percentage Area Chart'
			})
		})
]

export default testSteps

import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.stackedArea({
				x: 'Year',
				y: 'Value 2 (+)',
				stackedBy: 'Country',
				title: 'Stacked Area Chart'
			})
		})
]

export default testSteps

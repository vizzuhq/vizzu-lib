import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.splittedArea({
				x: 'Year',
				y: 'Value 2 (+)',
				splittedBy: 'Country',
				title: 'Splitted Area Chart'
			})
		})
]

export default testSteps

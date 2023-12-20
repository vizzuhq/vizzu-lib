import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,
			config: chart.constructor.presets.mekko({
				x: 'Value 1 (+)',
				y: 'Value 2 (+)',
				stackedBy: 'Joy factors',
				groupedBy: 'Country',
				title: 'Stacked Mekko Chart'
			})
		})
]

export default testSteps

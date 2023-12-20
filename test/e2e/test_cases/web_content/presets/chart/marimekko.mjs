import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,
			config: chart.constructor.presets.marimekko({
				x: 'Value 2 (+)',
				y: 'Value 3 (+)',
				stackedBy: 'Joy factors',
				groupedBy: 'Country',
				title: 'Marimekko Chart'
			})
		})
]

export default testSteps

import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.groupedColumn({
				x: 'Country',
				y: 'Value 5 (+/-)',
				groupedBy: 'Joy factors',
				title: 'Grouped Column Chart'
			})
		})
]

export default testSteps

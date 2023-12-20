import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.groupedBar({
				x: 'Value 5 (+/-)',
				y: 'Country',
				groupedBy: 'Joy factors',
				title: 'Grouped Bar Chart'
			})
		})
]

export default testSteps

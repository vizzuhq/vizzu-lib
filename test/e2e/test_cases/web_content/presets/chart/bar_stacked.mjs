import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.stackedBar({
				x: 'Value 2 (+)',
				y: 'Country',
				stackedBy: 'Joy factors',
				title: 'Stacked Bar Chart'
			})
		})
]

export default testSteps

import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.stackedColumn({
				x: 'Country',
				y: 'Value 2 (+)',
				stackedBy: 'Joy factors',
				title: 'Stacked Column Chart'
			})
		})
]

export default testSteps

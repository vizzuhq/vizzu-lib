import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.radialStackedBar({
				angle: 'Value 2 (+)',
				radius: 'Country',
				stackedBy: 'Joy factors',
				title: 'Radial Stacked Bar Chart'
			})
		})
]

export default testSteps

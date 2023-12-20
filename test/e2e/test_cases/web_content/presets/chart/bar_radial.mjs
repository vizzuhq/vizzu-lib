import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: chart.constructor.presets.radialBar({
				angle: 'Value 1 (+)',
				radius: 'Country',
				title: 'Radial Bar Chart'
			})
		})
]

export default testSteps

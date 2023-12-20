import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.polarScatter({
				angle: 'Value 3 (+)',
				radius: 'Value 2 (+)',
				dividedBy: 'Country',
				title: 'Polar Scatter Plot'
			})
		})
]

export default testSteps

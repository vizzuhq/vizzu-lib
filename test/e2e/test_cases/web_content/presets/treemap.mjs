import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.treemap({
				size: 'Value 2 (+)',
				color: 'Country_code',
				title: 'Treemap'
			})
		})
]

export default testSteps

import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.bubbleplot({
				x: 'Value 4 (+/-)',
				y: 'Value 5 (+/-)',
				color: 'Joy factors',
				dividedBy: 'Country',
				size: 'Value 3 (+)',
				title: 'Bubble Plot'
			})
		})
]

export default testSteps

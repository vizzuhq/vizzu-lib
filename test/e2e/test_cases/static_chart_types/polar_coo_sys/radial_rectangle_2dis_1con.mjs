import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { set: 'Joy factors', range: { min: '-30%' } },
					x: { set: ['Country', 'Value 2 (+)'], range: { max: '133%' } },
					color: 'Country'
				},
				title: 'Radial Chart (2disc)',
				coordSystem: 'polar'
			}
		})
]

export default testSteps

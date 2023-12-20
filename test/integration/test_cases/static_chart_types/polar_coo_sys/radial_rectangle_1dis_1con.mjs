import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { set: ['Joy factors'], range: { min: '-30%' } },
					x: { set: ['Value 2 (+)'], range: { max: '133%' } },
					color: 'Joy factors'
				},
				title: 'Radial Chart',
				coordSystem: 'polar'
			}
		})
]

export default testSteps

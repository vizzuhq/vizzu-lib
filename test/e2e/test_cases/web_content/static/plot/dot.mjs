import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Year', 'Value 5 (+/-)']
				},
				title: 'Dot Plot',
				geometry: 'circle'
			}
		})
]

export default testSteps

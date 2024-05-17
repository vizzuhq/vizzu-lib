import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: ['Country', 'Value 2 (+)'],
					x: 'Year',
					color: 'Country'
				},
				title: 'Area Spiderweb (fake. no multiaxes)',
				geometry: 'area',
				coordSystem: 'polar'
			}
		})
]

export default testSteps

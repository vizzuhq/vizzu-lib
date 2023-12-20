import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: 'Value 2 (+)',
					x: 'Year',
					color: 'Country'
				},
				title: 'Spiderweb (fake. no multiaxes)',
				geometry: 'line',
				coordSystem: 'polar'
			}
		})
]

export default testSteps

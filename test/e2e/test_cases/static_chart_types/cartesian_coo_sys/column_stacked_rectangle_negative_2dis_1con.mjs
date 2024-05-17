import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: ['Country', 'Year', 'Value 5 (+/-)'],
					x: 'Country_code',
					color: 'Country_code',
					lightness: 'Value 5 (+/-)'
				},
				title: 'Stacked Column Chart with (-) Numbers',
				legend: 'lightness'
			}
		})
]

export default testSteps

import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: ['Value 5 (+/-)', 'Country_code'],
					x: ['Year', 'Joy factors'],
					color: 'Country_code',
					size: ['Country_code', 'Value 2 (+)'],
					noop: 'Year'
				},
				title: 'Stacked Column Chart with (-) Nums'
			}
		})
]

export default testSteps

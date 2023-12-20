import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: 'Value 5 (+/-)',
					x: 'Year',
					color: 'Country'
				},
				title: 'Line Chart with (-) Nums (2disc)',
				geometry: 'line'
			}
		})
]

export default testSteps

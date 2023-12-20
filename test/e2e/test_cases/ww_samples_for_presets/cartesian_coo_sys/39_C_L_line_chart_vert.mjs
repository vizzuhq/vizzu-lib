import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					record.Country === 'Austria' ||
					record.Country === 'Belgium' ||
					record.Country === 'Bulgaria' ||
					record.Country === 'Cyprus' ||
					record.Country === 'Germany' ||
					record.Country === 'Denmark'
			}),
			config: {
				channels: {
					y: 'Year',
					x: 'Value 6 (+/-)',
					color: 'Country'
				},
				title: 'Line Chart vertical with (-)',
				geometry: 'line'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

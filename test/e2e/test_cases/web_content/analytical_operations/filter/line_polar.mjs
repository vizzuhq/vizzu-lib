import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- filter the Diemsion data series which is on the Color channel`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,
			config: {
				channels: {
					x: 'Year',
					y: 'Value 3 (+)',
					color: 'Country_code'
				},

				geometry: 'line',
				coordSystem: 'polar'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => record.Country_code === 'CY' || record.Country_code === 'ES'
			},
			config: {}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

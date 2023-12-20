import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- filter the Diemsion data series which is on the Y-axis`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,
			config: {
				channels: {
					x: 'Year',
					y: ['Value 2 (+)', 'Country_code'],
					color: 'Country_code'
				},

				geometry: 'area',
				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			data: {
				filter: (record) => record.Country_code === 'FR' || record.Country_code === 'CY'
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

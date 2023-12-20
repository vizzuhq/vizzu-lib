import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- set Split parameter from true to false`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: ['Value 2 (+)', 'Country'],
					color: 'Country'
				},

				geometry: 'area',
				split: true
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				split: false
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

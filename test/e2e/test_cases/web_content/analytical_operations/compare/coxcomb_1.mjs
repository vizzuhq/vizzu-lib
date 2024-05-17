import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- move the 2nd Dimension from X-axis to the Y-axis
- set the Split parameter to true`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: ['Country', 'Year'],
					y: { set: 'Value 2 (+)', range: { min: '-20%' } },
					color: 'Country'
				},

				coordSystem: 'polar'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)']
				},

				split: true
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

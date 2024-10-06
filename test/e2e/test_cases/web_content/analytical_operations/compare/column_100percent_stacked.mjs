import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const title = '100% Stacked Column'
const description = `- move the Dimension from the Y-axis to the X-axis, behind the one there (group)
- set the Align parameter from 'stretch' to 'min'`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				},

				align: 'stretch'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Year', 'Country'],
					y: 'Value 2 (+)'
				},

				align: 'none'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { title }
export { description }
export default testSteps

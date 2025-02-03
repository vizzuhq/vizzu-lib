import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- move the 2nd dimension from the X-axis to the Y-axis
- set the Align parameter to 'stretch'`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					x: ['Year', 'Country'],
					y: 'Value 2 (+)',
					color: 'Country'
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: { set: ['Country', 'Value 2 (+)'], align: 'stretch' }
				}
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove the Dimension from the X-axis
- shift the Meassure from the Y-axis to the X-axis
- set the Orientation to vertical`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Country',
					y: 'Value 2 (+)',
					label: 'Value 2 (+)'
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 2 (+)',
					y: null
				},

				orientation: 'vertical'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

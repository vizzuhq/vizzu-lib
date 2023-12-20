import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove the Dimension from the Y-axis
- switch the Geometry from Rectangle to Line`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 2 (+)'
				},

				geometry: 'line'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

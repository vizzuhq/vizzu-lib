import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove the Dimension from the X-axis`
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
					x: null
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

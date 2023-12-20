import { data_14 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- move the Measure from the Size channel to the Y-axis`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,

			config: {
				channels: {
					noop: ['Year', 'Country'],
					size: 'Value 2 (+)'
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					noop: ['Year', 'Country'],
					y: 'Value 2 (+)',
					size: null
				}
			}
		})
]

export { description }
export default testSteps

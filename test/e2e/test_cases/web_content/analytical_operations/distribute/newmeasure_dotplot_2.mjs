import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- add a new Measure to the X-axis`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					y: 'Value 2 (+)',
					noop: 'Year',
					color: 'Country'
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 3 (+)'
				}
			}
		})
]

export { description }
export default testSteps

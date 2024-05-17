import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- replace the Dimension on the X-axis with a new one`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)',
					color: 'Country'
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Joy factors'
					//                label: 'Country'
				}
			}
		})
]

export { description }
export default testSteps

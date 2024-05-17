import { data } from '../../../../test_data/chart_types_eu.mjs'

const description = `- add the Dimension whitch is on Label channel to the Size channel too (stack)`
const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: 'Joy factors',
					size: 'Value 2 (+)',
					label: 'Country_code'
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: ['Country_code', 'Value 2 (+)']
				}
			}
		})
]

export { description }
export default testSteps

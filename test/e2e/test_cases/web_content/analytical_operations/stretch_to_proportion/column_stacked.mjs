import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- set the Align parameter to 'stretch'`
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
				y: { align: 'stretch' }
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

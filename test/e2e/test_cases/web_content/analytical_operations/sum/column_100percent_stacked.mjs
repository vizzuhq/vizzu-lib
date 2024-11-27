import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const title = '100% Stacked Column'
const description = `- set Align parameter from 'stretch' to 'min'`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: {
						set: ['Country', 'Value 2 (+)'],

						align: 'stretch'
					},
					color: 'Country'
				}
			}
		}),

	(chart) =>
		chart.animate({
			y: {
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

import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove that Dimension from the X-axis, whitch is not on the Color channel
- move the Measure from Y-axis to the X-axis
- add the Measure to the Label channel too`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					x: ['Country', 'Year'],
					y: 'Value 2 (+)',
					color: 'Country'
				},
				title: 'Title'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: null,
					label: 'Value 2 (+)'
				},
				title: 'Title'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

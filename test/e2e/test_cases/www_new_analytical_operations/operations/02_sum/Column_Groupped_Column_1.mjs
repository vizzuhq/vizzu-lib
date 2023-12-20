import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove that Dimension from the X-axis, whitch is not on the Color channel
- shift the other Dimension from X-axis to the Y-axis
- add the Measure to the Label channel too`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: ['Year', 'Country'],
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
					x: null,
					y: ['Country', 'Value 2 (+)'],
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

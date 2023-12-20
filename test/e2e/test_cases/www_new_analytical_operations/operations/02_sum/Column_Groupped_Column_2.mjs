import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove the Dimension from the X-axis and the Color channel
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
					color: null,
					label: 'Value 2 (+)'
				},
				title: 'Title',
				legend: null
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

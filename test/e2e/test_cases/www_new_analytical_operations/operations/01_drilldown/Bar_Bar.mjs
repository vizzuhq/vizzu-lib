import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- add a new Dimension to the X-axis
- shift the Measure from X to the Y-axis`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Value 2 (+)',
					y: 'Country',
					color: 'Country',
					label: 'Value 2 (+)'
				},
				title: 'Title'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					label: null
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

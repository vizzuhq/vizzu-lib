import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove the Dimension from the X-axis
- set the Split parameter from true to false`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: ['Value 1 (+)', 'Country'],
					y: 'Value 3 (+)',
					noop: 'Year',
					color: 'Country'
				},

				geometry: 'circle',
				split: true,
				orientation: 'vertical'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 1 (+)'
				},

				split: false
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

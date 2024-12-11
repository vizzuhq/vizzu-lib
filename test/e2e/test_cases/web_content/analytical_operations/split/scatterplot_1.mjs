import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- add the Dimension which is on the Color channel to the X-axis too
- set the Orientation parameter to 'vertical'
- set the Split parameter to true`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					x: 'Value 2 (+)',
					y: 'Value 3 (+)',
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
					x: { set: ['Value 2 (+)', 'Country'], split: true }
				},

				geometry: 'circle',
				orientation: 'vertical'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

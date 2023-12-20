import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const title = '100% Stacked Area'
const description = `- remove the Dimension from Y-axis
- set the Align parameter from 'stretch' to 'min'
- switch the Geometry from Area to Line`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: ['Value 2 (+)', 'Country'],
					color: 'Country'
				},

				geometry: 'area',
				align: 'stretch'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 2 (+)'
				},

				geometry: 'line',
				align: 'min'
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

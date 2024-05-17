import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- add the Dimension which is on the Color channel to the Y-axis too
- switch the Geometry from Line to Area
- set the Split parameter to true`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)',
					color: 'Country'
				},

				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: ['Value 2 (+)', 'Country']
				},

				geometry: 'area',
				split: true
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

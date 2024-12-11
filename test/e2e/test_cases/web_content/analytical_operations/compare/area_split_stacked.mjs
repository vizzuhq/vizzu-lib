import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove the Dimension from Y-axis a
- set the Split parameter from true to false
- switch the Geometry from Area to Line`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Value 2 (+)', 'Country'], split: true },
					color: 'Country'
				},

				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: 'Value 2 (+)', split: false }
				},

				geometry: 'line'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

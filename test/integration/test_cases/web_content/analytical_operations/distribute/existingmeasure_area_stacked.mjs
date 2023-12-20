import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove the Dimension from the Y-axis
- switch the Geometry from Area to Circle`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				},

				geometry: 'area'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 2 (+)'
				},

				geometry: 'circle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

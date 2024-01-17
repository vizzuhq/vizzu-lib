import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- add a new Dimension to the Y-axis and the Color channel`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)'
				},

				geometry: 'area',
				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: ['Value 2 (+)', 'Country'],
					color: 'Country'
				}
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

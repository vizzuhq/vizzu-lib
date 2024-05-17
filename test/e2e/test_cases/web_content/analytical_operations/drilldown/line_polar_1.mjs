import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- add a new Dimension to the Y-axis & the Color channel
- switch the Geometry from Line to Area`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,
			config: {
				channels: {
					x: 'Year',
					y: 'Value 3 (+)'
				},

				geometry: 'line',
				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: ['Country', 'Value 3 (+)'],
					color: 'Country'
				},

				geometry: 'area'
			}
		})
]

export { description }
export default testSteps

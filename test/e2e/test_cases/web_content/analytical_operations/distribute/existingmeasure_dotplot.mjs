import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- replace the Dimension on the Y-axis with the Measure from the X-axis
- the other Dimension, which is on the no-operation (Noop) channel, is added to the X-axis also`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Value 3 (+)',
					y: 'Country',
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
					x: 'Year',
					y: 'Value 3 (+)'
				},

				orientation: 'horizontal'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

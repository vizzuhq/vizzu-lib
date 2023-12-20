import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- move the Dimension from the X-axis & replace the Measure with it on the Label channel
- add this Dimension to the Color channel too
- add a new Measure to the X-axis
- switch the Geometry from Rectangle (default) to Circe`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Country',
					y: 'Value 2 (+)',
					label: 'Value 2 (+)'
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 4 (+/-)',
					color: 'Country',
					label: 'Country'
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

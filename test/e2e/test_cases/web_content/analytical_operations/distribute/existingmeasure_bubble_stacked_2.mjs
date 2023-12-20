import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- move the Size channel's Dimension to the X-axis
- move the Measure to the Y-axis
- duplicate the Dimension in the Color channel onto the Y-axis
- null the Label channel
- switch the Geometry from Circle to Rectangle`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					color: 'Country',
					size: ['Year', 'Value 2 (+)'],
					label: 'Value 2 (+)'
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					size: null,
					label: null
				},

				geometry: 'rectangle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

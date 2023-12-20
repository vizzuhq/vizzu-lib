import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- Move the Size channel's Dimension to the X-axis
- Move the Measure to the Y-axis
- duplicate the Dimension in the Color channel onto the Y-axis
- Null the Label channel
- switch the Geometry from Rectangle to Area`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: null,
					y: null,
					color: 'Country',
					size: ['Year', 'Value 3 (+)'],
					label: 'Year'
				},
				title: 'Title',
				geometry: 'rectangle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country',
					size: null,
					label: null
				},
				title: 'Title',
				geometry: 'area'
			}
		}),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

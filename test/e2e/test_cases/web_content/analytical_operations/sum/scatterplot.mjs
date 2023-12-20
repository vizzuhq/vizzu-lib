import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove the Dimension from the Label channel and add the othere Dimension to it`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					color: 'Country',
					label: 'Year'
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					label: 'Country'
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

import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- replace the Dimension on the Y-axis to a new Measure`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_4,

			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Country',
					noop: ['Year'],
					color: 'Country'
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 3 (+)'
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

import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- move the Dimension from X-axis to the Y-axis and the Color channel`
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
					x: null,
					y: ['Country', 'Value 2 (+)'],
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

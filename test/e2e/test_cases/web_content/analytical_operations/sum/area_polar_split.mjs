import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- set Split parameter from true to false`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Country', 'Value 2 (+)'], range: { min: '-20%' }, split: true },
					color: 'Country'
				},
				geometry: 'area',
				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				y: { split: false }
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

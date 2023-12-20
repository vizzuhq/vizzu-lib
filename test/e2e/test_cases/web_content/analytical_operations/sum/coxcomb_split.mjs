import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- set Split parameter from true to false`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Country', 'Value 2 (+)'], range: { min: '-20%' } },
					color: 'Country'
				},

				split: true,
				coordSystem: 'polar'
			},
			style: {
				plot: {
					marker: {
						rectangleSpacing: '0.1em'
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				split: false
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

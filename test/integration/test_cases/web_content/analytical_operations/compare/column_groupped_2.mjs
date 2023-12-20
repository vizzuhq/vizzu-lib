import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- move the 2nd Dimension from X-axis to the Y-axis
- add the Measure to the Label channel too
- set the Split parameter to true`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: ['Country', 'Year'],
					y: 'Value 2 (+)',
					color: 'Country'
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					label: 'Value 2 (+)'
				},

				split: true
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top',
							fontSize: '0.6em'
						}
					}
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

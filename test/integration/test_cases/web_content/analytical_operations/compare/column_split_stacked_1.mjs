import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- move the dimension from the Y-axis to the X-axis, in front of the one there
- null the Label channel
- set the Split parameter to false`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country',
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

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Year', 'Country'],
					y: 'Value 2 (+)',
					label: null
				},

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

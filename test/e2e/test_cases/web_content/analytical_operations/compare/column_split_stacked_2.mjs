import { data_6 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- move the dimension from the Y-axis to the X-axis, behind the one there
- null the Label channel
- set the Split parameter to false`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_6,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Country', 'Value 2 (+)'], split: true },
					color: 'Country',
					label: 'Value 2 (+)'
				}
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
					x: ['Country', 'Year'],
					y: { set: 'Value 2 (+)', split: false },
					label: null
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

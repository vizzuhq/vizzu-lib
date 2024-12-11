import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const title = '100% Stacked Column'
const description = `- add the Measure to the Label channel too
- set the Align parameter from 'stretch' to 'min'
- set the Split parameter to true`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Year',
					y: { set: ['Country', 'Value 2 (+)'], align: 'stretch' },
					color: 'Country'
				}
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: {
						align: 'none',
						split: true
					},
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
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { title }
export { description }
export default testSteps

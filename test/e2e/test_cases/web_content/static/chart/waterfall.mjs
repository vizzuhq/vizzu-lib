import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					/* Adding the same dimension (Year) on both axes is
                how you create a Waterfall Chart in Vizzu. */
					y: ['Year', 'Value 5 (+/-)'],
					label: 'Value 5 (+/-)'
				},
				title: 'Waterfall Chart',
				legend: null
			},
			style: {
				plot: {
					marker: {
						label: {
							position: 'top'
						}
					}
				}
			}
		})
]

export default testSteps

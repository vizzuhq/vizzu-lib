import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Year', 'Value 5 (+/-)'],
					y: ['Country', 'Year'],
					color: 'Country',
					lightness: 'Value 6 (+/-)'
				},
				title: 'Sample Waterfall',
				orientation: 'vertical',
				legend: null
			},
			style: {
				plot: {
					yAxis: {
						label: {
							fontSize: 10
						}
					}
				}
			}
		})
]

export default testSteps

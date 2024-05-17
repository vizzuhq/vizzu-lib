import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: 'Country',
					x: ['Joy factors', 'Value 5 (+/-)'],
					color: 'Country',
					lightness: 'Joy factors'
				},
				title: 'Stacked Bar Chart',
				orientation: 'vertical',
				legend: null
			},
			style: {
				plot: {
					yAxis: {
						label: {
							fontSize: 11
						}
					}
				}
			}
		})
]

export default testSteps

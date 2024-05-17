import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: 'Country',
					x: 'Value 5 (+/-)',
					color: 'Country',
					label: 'Value 5 (+/-)'
				},
				title: 'Bar Chart with (-) Nums',
				orientation: 'vertical'
			},
			style: {
				plot: {
					yAxis: {
						label: {
							fontSize: 11
						}
					},
					marker: {
						label: {
							fontSize: 10
						}
					}
				}
			}
		})
]

export default testSteps

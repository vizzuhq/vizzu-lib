import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: { set: ['Year'], range: { min: '-20%' } },
					x: { set: ['Country', 'Value 2 (+)'], range: { max: '133%' } },
					color: 'Country',
					lightness: 'Year'
				},
				title: 'Stacked Radial Chart',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					yAxis: {
						label: {
							fontSize: 8
						}
					}
				}
			}
		})
]

export default testSteps

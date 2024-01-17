import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: {
						set: ['Value 2 (+)'],
						/* Set enough space for 
                    tangential labels. */
						range: { max: '130%' }
					},
					label: 'Value 2 (+)'
				},
				title: 'Polar Area Chart',
				geometry: 'area',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					marker: {
						label: {
							orientation: 'tangential',
							angle: 3.14 * -0.5
						}
					}
				}
			}
		})
]

export default testSteps

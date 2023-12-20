import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: { set: 'Value 2 (+)', range: { max: '130%' } },
					label: 'Value 2 (+)'
				},
				title: 'Spiderweb Area (not a real Spider)',
				legend: null,
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

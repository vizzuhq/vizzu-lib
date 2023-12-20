import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Area Chart',
				geometry: 'line',
				legend: null
			},
			style: {
				plot: {
					marker: {
						guides: {
							color: null,
							lineWidth: 0
						}
					}
				}
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year', 'Value 5 (+/-)'] },
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: ['Country'] }
					},
					title: 'Scatter plot',
					geometry: 'circle',
					coordSystem: 'polar',
					orientation: 'horizontal',
					split: false
				}
			},
			{
				geometry: {
					delay: 0,
					duration: 1
				},
				x: {
					delay: 0.5
				}
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year'] },
						y: { set: ['Value 2 (+)'] },
						color: { set: ['Country'] }
					},
					title: 'Stacked Area Chart',
					geometry: 'line',
					coordSystem: 'cartesian',
					legend: null
				}
			},
			{
				geometry: {
					delay: 1,
					duration: 1
				},
				y: {
					delay: 0.5
				}
			}
		)
]

export default testSteps

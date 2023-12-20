import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: 'Year',
					y: ['Value 2 (+)'],
					color: 'Country'
				},
				title: 'Spider Dotplot Fake',
				geometry: 'circle',
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
						x: 'Year',
						y: ['Value 2 (+)', 'Country'],
						color: 'Country'
					},
					title: 'Stacked Column Chart',
					geometry: 'rectangle',
					coordSystem: 'polar',
					//            orientation: 'horizontal',
					split: false
				}
			},
			{
				geometry: {
					delay: 0.7,
					duration: 1
				}
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: 'Value 5 (+/-)',
						noop: 'Year',
						y: 'Value 2 (+)',
						color: 'Country'
					},
					title: 'Spider Dotplot Fake',
					geometry: 'circle',
					coordSystem: 'cartesian',
					legend: null
				}
			},
			{
				geometry: {
					delay: 0,
					duration: 1
				}
			}
		)
]

export default testSteps

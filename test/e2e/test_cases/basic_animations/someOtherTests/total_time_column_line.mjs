import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: 'Value 2 (+)',
					x: 'Year',
					color: 'Joy factors'
				},
				title: 'Line Chart',
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						y: ['Value 2 (+)', 'Year'],
						x: 'Joy factors'
					},
					title: 'Column Chart',
					geometry: 'rectangle',
					sort: 'byValue'
				}
			},
			{
				/* Setting a custom rhythm for the animation
            to assist the viewer in following it. */
				geometry: {
					delay: 0.5,
					duration: 0.5
				},
				y: {
					delay: 0.5,
					duration: 1
				},
				x: {
					delay: 0,
					duration: 1
				}
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 2 (+)',
					label: 'Value 2 (+)'
				}
			}
		})
]

export default testSteps

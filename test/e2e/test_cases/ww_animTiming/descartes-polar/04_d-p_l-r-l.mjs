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
				title: 'Line Chart',
				geometry: 'line',
				legend: null
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year'] },
						y: { set: ['Value 2 (+)', 'Country'] },
						color: { set: ['Country'] }
					},
					title: 'Coxcomb',
					geometry: 'rectangle',
					coordSystem: 'polar'
				}
			},
			{
				geometry: {
					delay: 0.5,
					duration: 1.5
				},
				x: {
					delay: 1
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
					title: 'Line Chart',
					geometry: 'line',
					coordSystem: 'cartesian',
					legend: null
				}
			},
			{
				geometry: {
					delay: 0,
					duration: 1.5
				},
				y: {
					delay: 1
				}
			}
		)
]

export default testSteps

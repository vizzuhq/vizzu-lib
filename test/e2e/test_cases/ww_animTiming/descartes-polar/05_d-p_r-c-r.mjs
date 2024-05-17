import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Value 2 (+)', 'Country'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Column Chart',
				geometry: 'rectangle',
				legend: null
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Value 4 (+/-)'] },
						y: { set: ['Value 2 (+)'] },
						noop: { set: ['Year'] },
						color: { set: ['Country'] }
					},
					title: 'Spider Dotplot Fake',
					geometry: 'circle',
					coordSystem: 'polar',
					split: false
				}
			},
			{
				geometry: {
					delay: 0,
					duration: 1
				}
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Year'] },
						y: { set: ['Country', 'Value 2 (+)'] },
						color: { set: ['Country'] }
					},
					title: 'Stacked Column Chart',
					geometry: 'rectangle',
					coordSystem: 'cartesian',
					orientation: 'horizontal',
					legend: null
				}
			},
			{
				geometry: {
					delay: 0.7,
					duration: 1
				}
			}
		)
]

export default testSteps

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
						y: { set: ['Country', 'Value 2 (+)'], split: true }
					},
					title: 'Stacked Line Chart'
				}
			},
			{
				geometry: {
					delay: 0,
					duration: 0.75
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 0.5
					//               easing: 'ease-in'
				},
				y: {
					delay: 0,
					duration: 0.5
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Value 2 (+)', 'Year'] },
						y: { set: ['Country'], split: false },
						color: { set: ['Country'] }
					},
					title: 'Bar Chart',
					geometry: 'rectangle'
				}
			},
			{
				geometry: {
					delay: 0.25,
					duration: 0.75
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 0.75
					//               easing: 'ease-in'
				},
				y: {
					delay: 0,
					duration: 0.75
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
			}
		)
]

export default testSteps

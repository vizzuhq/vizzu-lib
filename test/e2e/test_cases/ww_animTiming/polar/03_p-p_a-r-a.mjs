import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Polar Area',
				coordSystem: 'polar',
				geometry: 'area',
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
					title: 'Stacked Coxcomb',
					geometry: 'rectangle'
				}
			},
			{
				geometry: {
					delay: 0.7,
					duration: 0.5
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
						y: { set: ['Country', 'Value 2 (+)'] },
						color: { set: ['Country'] }
					},
					title: 'Stacked Polar Area',
					geometry: 'area',
					legend: null
				}
			},
			{
				easing: 'cubic-bezier(0.65,0,0.65,1)',
				geometry: {
					delay: 0,
					duration: 1
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 1
					//               easing: 'ease-in'
				},
				y: {
					delay: 0,
					duration: 1
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
			}
		)
]

export default testSteps

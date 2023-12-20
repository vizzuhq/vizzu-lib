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
				title: 'Polar Line Chart',
				coordSystem: 'polar',
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
					title: 'Stacked Coxcomb',
					geometry: 'rectangle',
					//            orientation: 'horizontal',
					split: false
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
						x: { set: ['Year'] },
						y: { set: ['Value 2 (+)'] },
						color: { set: ['Country'] }
					},
					title: 'Polar Line Chart',
					geometry: 'line',
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
					delay: 0.5,
					duration: 0.5
					//               easing: 'ease-in'
				},
				y: {
					delay: 0.5,
					duration: 0.5
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
			}
		)
]

export default testSteps

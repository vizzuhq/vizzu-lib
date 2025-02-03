import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: { set: ['Year'] },
					y: { set: ['Value 2 (+)'] },
					noop: { set: ['Country'] },
					color: { set: ['Country'] }
				},
				title: 'Dotplot',
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
						x: { set: ['Year', 'Value 2 (+)'] },
						y: { set: ['Country'] },
						color: { set: ['Country'] }
					},
					title: 'Radial Area Chart Vertical',
					geometry: 'area',
					coordSystem: 'polar'
				}
			},
			{
				easing: 'cubic-bezier(0.65,0,0.65,1)',
				coordSystem: {
					delay: 0,
					duration: 1
				},
				geometry: {
					delay: 0.5,
					duration: 0.5
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
		),

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
					title: 'Dotplot',
					geometry: 'circle',
					coordSystem: 'cartesian',
					legend: null
				}
			},
			{
				easing: 'cubic-bezier(0.65,0,0.65,1)',
				coordSystem: {
					delay: 0,
					duration: 1
				},
				geometry: {
					delay: 0,
					duration: 0.5
					//                easing: 'linear'
				},
				x: {
					delay: 0.25,
					duration: 0.75
					//               easing: 'ease-in'
				},
				y: {
					delay: 0.25,
					duration: 0.75
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
			}
		)
]

export default testSteps

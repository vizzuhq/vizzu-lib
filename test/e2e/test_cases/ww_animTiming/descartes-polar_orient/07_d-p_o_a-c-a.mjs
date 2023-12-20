import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate(
			{
				data: data_14,
				config: {
					channels: {
						x: { set: ['Year'] },
						y: { set: ['Country', 'Value 2 (+)'] },
						color: { set: ['Country'] }
					},
					title: 'Stacked Area Chart',
					geometry: 'area',
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
			},
			{
				title: {
					delay: 0,
					duration: 1,
					easing: 'ease-in-out'
				}
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Value 2 (+)'] },
						noop: { set: ['Year'] },
						y: { set: ['Country'] },
						color: { set: ['Country'] }
					},
					title: 'Spider Dot Chart',
					geometry: 'circle',
					coordSystem: 'polar',
					orientation: 'horizontal',
					split: false
				}
			},
			{
				easing: 'cubic-bezier(0.65,0,0.65,1)',
				title: {
					delay: 0,
					duration: 1,
					easing: 'ease-in-out'
				},
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
						x: { set: ['Year'] },
						y: { set: ['Country', 'Value 2 (+)'] },
						color: { set: ['Country'] }
					},
					title: 'Stacked Area Chart',
					coordSystem: 'cartesian',
					geometry: 'area',
					legend: null
				}
			},
			{
				easing: 'cubic-bezier(0.65,0,0.65,1)',
				title: {
					delay: 0,
					duration: 1,
					easing: 'ease-in-out'
				},
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
					delay: 0.25,
					duration: 0.75
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
			}
		)
]

export default testSteps

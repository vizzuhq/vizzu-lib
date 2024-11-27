import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					record.Country === 'Austria' ||
					record.Country === 'Belgium' ||
					record.Country === 'Bulgaria' ||
					record.Country === 'Cyprus' ||
					record.Country === 'Czechia' ||
					record.Country === 'Denmark' ||
					record.Country === 'Estonia' ||
					record.Country === 'Greece' ||
					record.Country === 'Germany' ||
					//               record.Country === 'Spain' ||
					//               record.Country === 'Finland' ||
					//               record.Country === 'France' ||
					//               record.Country === 'Croatia' ||
					record.Country === 'Hungary'
			}),
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
						y: { set: ['Country', 'Value 2 (+)'] },
						color: { set: ['Country'] }
					},
					title: 'Radial Stacked Area Chart Vertical',
					geometry: 'area',
					coordSystem: 'polar'
				}
			},
			{
				easing: 'cubic-bezier(0.65,0,0.65,1)',
				coordSystem: {
					delay: 0,
					duration: 1
					//                easing: 'linear'
				},
				geometry: {
					delay: 0,
					duration: 0.5
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 0
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
					title: 'Line Chart',
					geometry: 'line',
					coordSystem: 'cartesian',
					legend: null
				}
			},
			{
				easing: 'cubic-bezier(0.65,0,0.65,1)',
				coordSystem: {
					delay: 0,
					duration: 1
					//                easing: 'linear'
				},
				geometry: {
					delay: 0.5,
					duration: 0.5
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 0
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

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
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: ['Country'] }
				},
				title: 'Stacked Area Chart',
				geometry: 'area',
				legend: null
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						y: { range: { max: '100%' } }
					},
					title: 'Splitted Area Chart',
					align: 'center',
					split: true
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
						x: { set: ['Value 2 (+)', 'Year'] },
						y: { set: ['Country'] },
						color: { set: ['Country'] }
					},
					title: 'Bar Chart',
					geometry: 'rectangle',
					align: 'min',
					split: false
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
						y: { set: ['Country', 'Value 2 (+)'] }
					},
					title: 'Splitted Area Chart',
					geometry: 'area',
					align: 'center',
					split: true
				}
			},
			{
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
					delay: 0.75,
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
						y: { range: { max: '110%' } }
					},
					title: 'Stacked Area Chart',
					geometry: 'area',
					legend: null,
					align: 'min',
					split: false
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

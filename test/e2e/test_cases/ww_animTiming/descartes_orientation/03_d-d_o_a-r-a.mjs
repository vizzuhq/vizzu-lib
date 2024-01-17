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
						x: { set: ['Value 2 (+)', 'Year'] },
						y: { set: ['Country'] },
						color: { set: ['Country'] }
					},
					title: 'Bar Chart',
					geometry: 'rectangle',
					split: false
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
					title: 'Stacked Area Chart',
					geometry: 'area',
					legend: null
				}
			},
			{
				geometry: {
					delay: 0,
					duration: 0.5
					//                easing: 'linear'
				},
				x: {
					delay: 0,
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

import { data } from '../../../../../test_data/chart_types_eu.mjs'

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
					record.Country === 'Spain' ||
					record.Country === 'Finland' ||
					record.Country === 'France' ||
					record.Country === 'Croatia' ||
					record.Country === 'Hungary'
			}),
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: 'Country' }
				},
				title: 'Stacked Column Chart',
				geometry: 'rectangle'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'Country' },
						color: { set: 'Country' }
					},
					title: 'Remove Conti'
				},
				style: {
					plot: {
						marker: { rectangleSpacing: '0' }
					}
				}
			},
			{
				easing: 'cubic-bezier(.39,0,.35,.99)',
				y: {
					delay: 0
				}
			}
		),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Joy factors' },
						y: { set: 'Country' },
						color: { set: 'Country' }
					},
					title: 'Change Discrete'
				}
			},
			{
				duration: 0
			}
		),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					color: { set: 'Country' },
					size: { set: ['Joy factors', 'Value 2 (+)'] }
				},
				title: 'Add previous Conti & Change CoordSys'
			}
		})
]

export default testSteps

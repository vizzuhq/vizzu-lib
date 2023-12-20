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
					x: { set: ['Year', 'Value 3 (+)'] },
					y: { set: ['Country', 'Value 2 (+)'] },
					color: { set: 'Country' }
				},
				title: 'Mekko Chart',
				orientation: 'horizontal'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Value 3 (+)' },
					y: { set: ['Year', 'Country', 'Value 2 (+)'] },
					color: null
				},
				title: 'Stack Disc & Remove Color & Stretch & axisLabel off',
				align: 'stretch'
			},
			style: {
				plot: {
					xAxis: { label: { color: 'rgb(255,255,255)' } }
				}
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Value 3 (+)' },
						y: {
							set: ['Joy factors', 'Value 2 (+)'],
							range: { min: '0%', max: '100%' }
						}
					},
					title: 'Change Disc',
					orientation: 'auto'
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
					color: { set: 'Joy factors' },
					lightness: { set: 'Value 2 (+)' },
					size: { set: 'Value 3 (+)' }
				},
				title: 'add new Disc Color & Change CoordSys'
			}
		})
]

export default testSteps

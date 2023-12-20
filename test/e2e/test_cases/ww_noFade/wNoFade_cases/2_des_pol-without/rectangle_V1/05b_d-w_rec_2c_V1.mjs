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
					x: { set: ['Joy factors', 'Value 1 (+)'] },
					y: { set: ['Country', 'Value 3 (+)'] },
					color: { set: 'Country' }
				},
				title: 'Mekko Chart',
				geometry: 'rectangle',
				orientation: 'horizontal'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Joy factors', 'Value 1 (+)'] },
					y: { set: 'Value 3 (+)' },
					color: null
				},
				title: 'Remove Y Disc & Remove Color'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: ['Joy factors', 'Value 1 (+)'] },
						y: { set: ['Year', 'Value 3 (+)'] },
						color: null
					},
					title: 'Add new Disc',
					orientation: 'horizontal'
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
					color: { set: 'Year' },
					lightness: { set: 'Value 2 (+)' },
					size: { set: ['Joy factors', 'Value 3 (+)'] }
				},
				title: 'Add new Disc Color & Add new Conti & Change CoordSys'
			}
		})
]

export default testSteps

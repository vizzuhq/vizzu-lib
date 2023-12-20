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
				title: 'Stacked Mekko Chart',
				geometry: 'rectangle',
				orientation: 'horizontal'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: { set: ['Country', 'Year', 'Value 2 (+)'] }
				},
				title: 'Stack Disc'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: null,
						y: { set: ['Country', 'Joy factors', 'Value 2 (+)'] }
					},
					title: 'Change Disc'
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
					y: { set: ['Country', 'Joy factors', 'Value 1 (+)'] }
				},
				title: 'Change Conti'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: ['Joy factors', 'Value 3 (+)'] },
					y: { set: ['Country', 'Value 1 (+)'] }
				},
				title: 'Group new Disc',
				orientation: 'horizontal'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					color: { set: 'Country' },
					lightness: { set: 'Value 2 (+)' },
					size: { set: ['Joy factors', 'Value 1 (+)'] }
				},
				title: 'Change CoordSys'
			}
		})
]

export default testSteps

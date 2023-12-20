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
		chart.animate({
			config: {
				channels: {
					x: null,
					y: { set: ['Year', 'Country', 'Value 2 (+)'] },
					color: null
				},
				title: 'Stack Disc & Remove Color'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						y: { set: ['Joy factors', 'Value 2 (+)'] }
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
					x: { set: ['Joy factors'] },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Joy factors' }
				},
				title: 'Group new Disc & Add new Disc Color'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					color: { set: 'Joy factors' },
					size: { set: 'Value 2 (+)' }
				},
				title: 'Change CoordSys'
			}
		})
]

export default testSteps

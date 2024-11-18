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
					y: { set: 'Value 2 (+)' },
					color: { set: 'Year' }
				},
				title: 'Mekko Chart',
				geometry: 'rectangle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { set: 'Value 3 (+)' },
					y: { set: ['Year', 'Value 2 (+)'], labelLevel: 0 },
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
						x: { set: ['Value 3 (+)'] },
						y: { set: ['Joy factors', 'Value 2 (+)'], labelLevel: 0 }
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
					x: { set: ['Joy factors', 'Value 3 (+)'] },
					y: { set: 'Value 2 (+)', range: { max: '125%' } },
					color: { set: 'Joy factors' }
				},
				title: 'Group new Disc & Add new Disc Color & Stretch off & axisLabel on',
				align: 'none'
			},
			style: {
				plot: {
					xAxis: { label: { color: null } }
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					color: { set: 'Joy factors' },
					lightness: { set: 'Value 2 (+)' },
					size: { set: 'Value 1 (+)' }
				},
				title: 'Change CoordSys'
			}
		})
]

export default testSteps

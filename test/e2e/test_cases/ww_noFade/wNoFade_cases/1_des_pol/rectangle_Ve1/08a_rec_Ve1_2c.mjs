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
					x: { set: 'Value 1 (+)', align: 'stretch' },
					y: { set: ['Year', 'Value 2 (+)'], labelLevel: 0 },
					color: null
				},
				title: 'Stack Disc & Remove Color & change Conti & Stretch & axisLabel off'
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
						x: { set: ['Value 1 (+)'] },
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
					x: { set: ['Joy factors', 'Value 1 (+)'], align: 'none' },
					y: { set: 'Value 2 (+)' },
					color: { set: 'Joy factors' }
				},
				title: 'Group new Disc & Add new Disc Color & Stretch off & axisLabel on'
			},
			style: {
				plot: {
					xAxis: { label: { color: null } }
				}
			}
		})
]

export default testSteps

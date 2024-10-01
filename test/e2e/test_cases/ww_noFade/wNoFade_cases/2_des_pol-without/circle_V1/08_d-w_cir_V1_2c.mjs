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
					y: { set: 'Value 3 (+)' },
					x: { set: 'Value 2 (+)' },
					color: { set: 'Country' },
					noop: { set: 'Year' }
				},
				title: 'Scatter plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: { set: null, range: { min: '0%', max: '800%' } },
					x: { set: 'Value 2 (+)', range: { min: '0%', max: '200%' } },
					color: null,
					noop: null,
					size: { set: ['Year', 'Country'] }
				},
				title: 'Stack Discs'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						noop: null,
						size: { set: 'Joy factors' }
					},
					title: 'Change Discs'
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
					noop: { set: 'Joy factors' },
					size: { set: 'Value 1 (+)' }
				},
				title: 'Add new Conti & Change CoordSys'
			}
		})
]

export default testSteps

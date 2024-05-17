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
					y: 'Value 1 (+)',
					x: 'Value 2 (+)',
					color: 'Country',
					noop: 'Year'
				},
				title: 'Scatter plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 1 (+)',
					x: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors',
					noop: null
				},
				title: 'Group new Discrete & Change Geoms',
				geometry: 'rectangle',
				orientation: 'horizontal'
			}
		})
]

export default testSteps

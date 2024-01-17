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
					y: 'Value 3 (+)',
					x: 'Value 2 (+)',
					color: 'Country',
					noop: 'Year',
					size: 'Value 1 (+)'
				},
				title: 'Dot plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: ['Joy factors', 'Value 3 (+)'],
					noop: 'Joy factors',
					size: 'Value 1 (+)'
				},
				title: 'Change Geometry',
				geometry: 'rectangle',
				orientation: 'vertical'
			}
		})
]

export default testSteps

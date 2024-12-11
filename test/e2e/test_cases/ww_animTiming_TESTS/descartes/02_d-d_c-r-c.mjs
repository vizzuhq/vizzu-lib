import { data } from '../../../test_data/chart_types_eu.mjs'

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
					record.Country === 'Germany' ||
					record.Country === 'Denmark' ||
					record.Country === 'Estonia' ||
					record.Country === 'Greece' ||
					record.Country === 'Spain' ||
					record.Country === 'Finland' ||
					record.Country === 'France' ||
					record.Country === 'Croatia' ||
					record.Country === 'Hungary'
			}),

			config: {
				channels: {
					x: ['Value 5 (+/-)', 'Year'],
					y: 'Value 2 (+)',
					color: 'Country'
				},
				title: 'Stacked Area Chart',
				geometry: 'circle',
				legend: null
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: ['Country', 'Value 2 (+)']
				},
				title: 'Bar Chart',
				geometry: 'rectangle'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 4 (+/-)',
					y: 'Value 2 (+)',
					noop: 'Year',
					color: 'Country'
				},
				title: 'Stacked Area Chart',
				geometry: 'circle',
				legend: null
			}
		})
]

export default testSteps

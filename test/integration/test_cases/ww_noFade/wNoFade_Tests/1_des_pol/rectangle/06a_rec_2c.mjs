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
					x: ['Year', 'Value 3 (+)'],
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				},
				title: 'Stacked Mekko Chart',
				orientation: 'horizontal'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Joy factors', 'Value 1 (+)'],
					y: ['Country', 'Value 5 (+/-)']
				},
				title: 'Group new Disc',
				orientation: 'horizontal'
			}
		})
]

export default testSteps

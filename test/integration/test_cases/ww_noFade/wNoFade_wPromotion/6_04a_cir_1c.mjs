import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					(record.Country === 'Cyprus' ||
						record.Country === 'Czechia' ||
						record.Country === 'Denmark' ||
						record.Country === 'Germany' ||
						record.Country === 'Spain' ||
						record.Country === 'Finland') &&
					record.Year >= 10
			}),
			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)',
					color: 'Country'
				},
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Joy factors',
					color: 'Country'
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					color: 'Country',
					label: null
				}
			}
		})
]

export default testSteps

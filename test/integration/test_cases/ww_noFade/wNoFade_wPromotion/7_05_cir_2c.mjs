import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					(record.Country === 'Belgium' ||
						record.Country === 'Cyprus' ||
						record.Country === 'Denmark') &&
					record.Year >= 10
			}),
			config: {
				channels: {
					y: 'Value 3 (+)',
					x: 'Value 2 (+)',
					color: 'Country'
				},
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 3 (+)',
					noop: 'Year',
					label: 'Year'
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 3 (+)',
					color: 'Country',
					noop: null,
					label: null
				}
			}
		})
]

export default testSteps

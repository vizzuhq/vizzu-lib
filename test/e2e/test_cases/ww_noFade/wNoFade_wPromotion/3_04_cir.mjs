import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					(record.Country === 'Austria' ||
						record.Country === 'Belgium' ||
						record.Country === 'Cyprus' ||
						record.Country === 'Denmark' ||
						record.Country === 'Greece' ||
						record.Country === 'Germany') &&
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
					y: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors'
				},
				geometry: 'rectangle'
			},
			style: {
				plot: {
					marker: {
						colorPalette: '#ef675aFF #6d8cccFF #e6cf99FF #9c50abFF'
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 2 (+)',
					color: 'Country'
				},
				geometry: 'circle'
			},
			style: {
				plot: {
					marker: {
						colorPalette: null
					}
				}
			}
		})
]

export default testSteps

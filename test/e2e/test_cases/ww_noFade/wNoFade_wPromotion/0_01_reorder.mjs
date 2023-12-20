import { data_8 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data_8, {
				filter: (record) =>
					(record.Country === 'Austria' ||
						record.Country === 'Belgium' ||
						record.Country === 'Cyprus' ||
						record.Country === 'Denmark' ||
						record.Country === 'Greece' ||
						record.Country === 'Germany') &&
					record.Year >= 15
			})
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 3 (+)',
					y: 'Value 2 (+)',
					color: 'Joy factors',
					lightness: null,
					size: null,
					label: 'Country_code',
					noop: 'Country'
				},
				geometry: 'circle'
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
					x: 'Country',
					y: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors',
					lightness: null,
					size: null,
					label: null,
					noop: null
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
					x: null,
					y: null,
					color: 'Country',
					lightness: 'Value 3 (+)',
					size: ['Value 2 (+)'],
					label: 'Country_code',
					noop: null
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

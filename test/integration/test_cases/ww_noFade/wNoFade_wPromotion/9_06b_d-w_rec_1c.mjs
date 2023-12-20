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
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					color: 'Joy factors',
					size: ['Year', 'Value 2 (+)'],
					label: 'Year'
				}
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
					x: 'Year',
					y: ['Country', 'Value 2 (+)'],
					color: 'Country',
					size: null,
					label: null
				}
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

import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: Object.assign(data, {
				filter: (record) =>
					record.Country === 'Austria' ||
					record.Country === 'Belgium' ||
					record.Country === 'Cyprus' ||
					record.Country === 'Denmark' ||
					record.Country === 'Greece' ||
					record.Country === 'Germany'
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
					y: ['Joy factors', 'Value 1 (+)'],
					color: 'Joy factors'
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
					y: ['Country', 'Value 2 (+)'],
					color: 'Country'
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

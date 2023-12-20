import { data_14 } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_14,
			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)',
					color: 'Country'
				},
				geometry: 'line'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
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
			data: data_14,
			config: {
				channels: {
					x: 'Year',
					y: 'Value 2 (+)',
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

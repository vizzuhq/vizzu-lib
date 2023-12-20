import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Joy factors',
					y: 'Value 5 (+/-)',
					color: 'Joy factors',
					/* The noop channel (no operation) splits the markers as all the other channels
                but will have no effect on the markers’ appearance. */
					noop: 'Country_code'
				},
				title: 'Dot plot',
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
					x: ['Joy factors', 'Value 6 (+/-)'],
					label: 'Country_code'
				},
				title: 'Value Distribution with Additional Measure'
			}
		})
]

export default testSteps

import { data } from '../../../../test_data/chart_types_eu.mjs'

const description = `- add a new Measure to the X-axis
- move the Dimension from the no-operation (Noop) channel to the Label channel`
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
					label: 'Country_code',
					noop: null
				}
			}
		})
]

export { description }
export default testSteps

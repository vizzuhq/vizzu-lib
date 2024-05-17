import { data } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove the Dimension from the X-axis and Color channel
- shift the Measure from X to the Y-axis (change orientation), and add it to the Label channel`
const testSteps = [
	(chart) =>
		chart.animate({
			data,

			config: {
				channels: {
					x: 'Year',
					y: ['Joy factors', 'Value 2 (+)'],
					color: 'Joy factors'
				},

				coordSystem: 'polar'
			},
			style: {
				plot: {
					marker: {
						colorPalette: '#ef675aFF #6d8cccFF #e6cf99FF #9c50abFF',
						rectangleSpacing: '0.1em'
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 2 (+)',
					y: { set: 'Joy factors', range: { min: '-30%' } },
					label: 'Value 2 (+)'
				}
			},
			style: {
				plot: {
					marker: {
						rectangleSpacing: null
					}
				}
			}
		})
]

export { description }
export default testSteps

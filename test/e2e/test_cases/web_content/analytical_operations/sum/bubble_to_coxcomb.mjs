import { data } from '../../../../test_data/chart_types_eu.mjs'

const description = `This Vizzu animation consist of 2 steps (animates):

1st:
- move the Dimension from the Label channel to the Y-axis
- move the Measure from the Size channel to the Y-axis
- add the other Dimension to the X-axis
- switch the Geometry from Circle to Rectangle

2nd:
- remove the other Dimension from the X-axis 
- add the Measure to the Label channel too`
const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					color: 'Joy factors',
					label: 'Country_code',
					/* With a dimension on the size channel Vizzu will stack
                the elements by the categories on the other channels
                on charts without coordinates. Here the Country code dimension is
                used to stack the bubbles by the dimension on the color channel. */
					size: ['Country_code', 'Value 2 (+)']
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Joy factors',
					y: ['Country_code', 'Value 2 (+)'],
					label: null,
					/* The stacking is eliminated when we remove
                the extra dimension from the size channel. */
					size: null
				},

				geometry: 'rectangle',
				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Value 2 (+)',
					label: 'Value 2 (+)'
				}
			}
		})
]

export { description }
export default testSteps

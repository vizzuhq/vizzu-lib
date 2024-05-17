import { data } from '../../../../test_data/chart_types_eu.mjs'

const description = `This Vizzu animation consist of 2 steps (animates):

1st:
- remove two Measures from the axes
- move the Dimension from the Label channel and the third Measure from the Size channel to the X-axis
- switch the Geometry from Circle to Rectangle

2nd:
- remove the Dimension from the X-axis
- add the remaining Measure to the Label channel`
const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: ['Joy factors', 'Value 6 (+/-)'],
					y: 'Value 5 (+/-)',
					color: 'Joy factors',
					size: 'Value 2 (+)',
					label: 'Country_code'
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Country_code', 'Value 2 (+)'],
					y: 'Joy factors',
					label: null
				},

				geometry: 'rectangle',
				orientation: 'vertical'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 2 (+)',
					label: 'Value 2 (+)'
				}
			}
		})
]

export { description }
export default testSteps

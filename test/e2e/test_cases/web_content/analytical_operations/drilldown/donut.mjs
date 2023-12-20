import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `This Vizzu animation consist of 2 steps (animates):

1st:
- move the Dimension from X-axis to the Y-axis
- add a new Dimension to the X-axis
- remove the Measure from the label channel

2nd:
- move the Measure from X to Y-axis`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: ['Country', 'Value 2 (+)'],
					y: { range: { min: '-200%' } },
					color: 'Country',
					label: 'Value 2 (+)'
				},

				coordSystem: 'polar'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Year', 'Value 2 (+)'],
					y: { set: 'Country', range: { min: '-30%' } },
					label: null
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: { set: ['Country', 'Value 2 (+)'], range: { min: '-30%' } }
				}
			},
			style: {
				plot: {
					marker: {
						rectangleSpacing: '0.1em'
					}
				}
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

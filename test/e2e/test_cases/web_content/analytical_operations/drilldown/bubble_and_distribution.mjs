import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `This Vizzu animation consist of 2 steps (animates):

1st:
- add a new Sub Dimension to the Size channel

2nd:
- add two new Measures to the X and Y-axes
- move the new Sub Dimension to the No-Operation (Noop) channel
- null the Label channel`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					color: 'Country',
					size: 'Value 2 (+)',
					label: 'Value 2 (+)'
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					size: ['Year', 'Value 2 (+)']
				},

				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					noop: 'Year',
					size: 'Value 2 (+)',
					label: null
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

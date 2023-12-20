import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- remove two Measures from the axes
- remove Dimensions from the Color channel
- add the Measure to the Label channel too
- + if you like set the marker-label fontSize in Style`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,

			config: {
				channels: {
					x: 'Value 1 (+)',
					y: 'Value 3 (+)',
					noop: 'Year',
					size: 'Value 2 (+)',
					color: 'Country'
				},
				title: 'Title',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: null,
					y: null,
					noop: null,
					color: null,
					label: 'Value 2 (+)'
				},
				title: 'Title',
				legend: null,
				geometry: 'circle'
			},
			style: {
				plot: {
					marker: {
						label: {
							fontSize: '1.5em'
						}
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

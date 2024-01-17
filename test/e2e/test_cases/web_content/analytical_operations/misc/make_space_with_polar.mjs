import { data_8 } from '../../../../test_data/chart_types_eu.mjs'

const description = `- change coordinate system to 'polar'
- add left padding in the Style to make space near the chart for something`
const testSteps = [
	(chart) =>
		chart.animate({
			data: data_8,
			config: {
				channels: {
					x: 'Value 2 (+)',
					y: 'Country',
					color: 'Country'
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Value 2 (+)'],
					y: {
						set: 'Country',
						/* Setting the radius of
                    the empty circle in the centre. */
						range: {
							min: '-30%'
						}
					},
					size: null
				},

				coordSystem: 'polar'
			},
			style: {
				plot: {
					paddingLeft: '25em'
				}
			}
		})
]

export { description }
export default testSteps

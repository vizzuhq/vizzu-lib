import { data } from '../../../test_data/sunburst.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: {
						set: ['Joy factors/Country', 'Value 2 (+)'],
						range: { min: '0%', max: '100%' }
					},
					y: ['Type'],
					label: 'Joy factors/Country',
					color: 'Joy factors/Country'
				},
				title: 'Sunburst chart',
				orientation: 'vertical',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					marker: {
						rectangleSpacing: 0,
						label: {
							orientation: 'tangential',
							angle: 3.14 * -0.5
						}
					},
					yAxis: {
						title: {
							color: '#ffffff00'
						},
						label: {
							color: '#ffffff00'
						}
					}
				}
			}
		})
]

export default testSteps

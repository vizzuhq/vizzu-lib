import { data } from '../../../test_data/icicle.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: {
						set: ['Joy factors/Country', 'Value 2 (+)'],
						range: { max: '100%' }
					},
					y: { set: 'Type', title: 'Q. by Countries', range: { max: '100%' } },
					label: 'Joy factors/Country',
					color: 'Joy factors/Country'
				},
				title: 'Fake Icicle Chart'
			},
			style: {
				plot: {
					paddingLeft: '4.14em',
					yAxis: {
						ticks: { color: '#00000000' },
						label: { color: '#00000000' },
						color: '#00000000'
					},
					xAxis: {
						label: { color: '#00000000' },
						interlacing: { color: '#00000000' }
					},
					marker: {
						rectangleSpacing: '0',
						label: {
							orientation: 'vertical',
							angle: 3.14 * -1
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

export default testSteps

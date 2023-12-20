import { data } from '../../../test_data/funnel_data.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Num',
					y: 'Phase',
					color: 'Phase',
					label: ['Phase', 'Num']
				},
				title: 'Funnel',
				geometry: 'area',
				align: 'center'
			},
			style: {
				plot: {
					yAxis: {
						color: '#00000000',
						label: { color: '#00000000' }
					},
					xAxis: {
						color: '#00000000',
						title: { color: '#00000000' },
						label: { color: '#00000000' },
						interlacing: { color: '#00000000' }
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

import { data } from '../../../test_data/music_industry_history_1.mjs'

const testSteps = [
	(chart) =>
		chart.animate(
			{
				data,
				config: {
					channels: {
						x: {
							set: ['Revenue', 'Format'],
							range: { min: '-1%', max: '110%' },
							align: 'center',
							split: true
						},
						y: 'Year',
						color: 'Format'
					},
					title: 'Violin',
					geometry: 'area',
					orientation: 'vertical'
				},
				style: {
					plot: {
						yAxis: { label: { numberScale: 'K, M, B, T' } }
					}
				}
			},
			{
				delay: 0
			}
		),
	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: 'Format',
						y: 'Year',
						color: 'Format',
						size: 'Revenue'
					},
					title: 'Violin',
					geometry: 'circle',
					orientation: 'vertical'
				}
			},
			{
				delay: 1
			}
		),

	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

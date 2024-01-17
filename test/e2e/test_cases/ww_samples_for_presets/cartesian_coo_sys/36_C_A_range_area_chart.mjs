import { data_3 } from '../../../test_data/music_industry_history_1.mjs'

const testSteps = [
	(chart) => {
		chart.on('plot-axis-label-draw', (event) => {
			const year = parseFloat(event.detail.text)
			if (!isNaN(year) && year % 5 !== 0) event.preventDefault()
		})
		return chart.animate({
			data: data_3,
			config: {
				channels: {
					x: 'Year',
					y: ['Format', 'Revenue'],
					color: 'Format'
				},
				title: 'Range area chart',
				geometry: 'area',
				align: 'min',
				legend: null
			},
			style: {
				plot: {
					xAxis: {
						label: {
							angle: 0
						}
					},
					yAxis: {
						label: {
							numberScale: 'K, M, B, T'
						}
					}
				}
			}
		})
	},
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

import { data } from '../../../test_data/music_industry_history_1.mjs'

const testSteps = [
	(chart) => {
		chart.on('plot-axis-label-draw', (event) => {
			const year = parseFloat(event.detail.text)
			if (!isNaN(year) && year % 5 !== 0) event.preventDefault()
		})
		return chart.animate({
			data,
			config: {
				channels: {
					x: ['Format', 'Revenue'],
					y: 'Year',
					color: 'Format'
				},
				title: 'Violin Graph Vertical',
				geometry: 'area',
				align: 'center',
				split: true
			},
			style: {
				plot: {
					xAxis: {
						interlacing: { color: '#ffffff00' },
						label: { numberScale: 'K, M, B, T' }
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

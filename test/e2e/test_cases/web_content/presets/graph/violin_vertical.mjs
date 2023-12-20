import { data } from '../../../../test_data/music_industry_history_1.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: chart.constructor.presets.verticalViolin({
				x: 'Revenue',
				y: 'Year',
				splittedBy: 'Format',
				title: 'Vertical Violin Graph'
			}),
			style: {
				plot: {
					xAxis: { interlacing: { color: '#ffffff00' } },
					yAxis: { label: { numberScale: 'K, M, B, T' } }
				}
			}
		})
]

export default testSteps

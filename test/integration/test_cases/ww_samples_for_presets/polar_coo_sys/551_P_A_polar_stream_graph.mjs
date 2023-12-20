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
				title: 'Polar Stream Graph',
				geometry: 'area',
				coordSystem: 'polar',
				align: 'center',
				split: true
			}
		})
	},
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

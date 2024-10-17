import { data } from '../../../../test_data/music_industry_history_1.mjs'

const description = `This is a 2-step animation:

1st: 
- set the Split parameter to true
- set the Align parameter to 'min'

2nd:
- move the Dimension from the Y-axis to the X-axis (before the other)
- set the Split parameter to false`
const testSteps = [
	(chart) => {
		chart.on('plot-axis-label-draw', (event) => {
			const year = parseFloat(event.detail.text)
			if (!event.detail.text.includes('$') && !isNaN(year) && year % 5 !== 0)
				event.preventDefault()
		})
		return chart
	},

	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: ['Revenue', 'Format'],
					color: 'Format'
				},

				geometry: 'area',
				align: 'center'
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
		}),

	(chart) =>
		chart.animate({
			config: {
				align: 'none',
				split: true
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Format', 'Year'],
					y: 'Revenue'
				},

				split: false
			},
			style: {
				plot: {
					xAxis: {
						label: {
							angle: null
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

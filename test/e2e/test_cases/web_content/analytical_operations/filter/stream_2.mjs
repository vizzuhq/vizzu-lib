import { data } from '../../../../test_data/music_industry_history_1.mjs'

const description = `- fix the X-axis-range
- filter the Diemsion data series on the Y-axis step-by-step`
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
			data: Object.assign(data, {
				filter: (record) => record.Format === 'Tapes'
			}),
			config: {
				channels: {
					x: { set: 'Year', range: { max: '48' } }, // 48 = years in this time period
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
			data: {
				filter: (record) => record.Format === 'Tapes' || record.Format === 'Vinyl'
			},
			config: {}
		}),

	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record.Format === 'Tapes' ||
					record.Format === 'Cassette' ||
					record.Format === 'Vinyl'
			},
			config: {}
		}),

	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record.Format === 'DVD' ||
					record.Format === 'Tapes' ||
					record.Format === 'Cassette' ||
					record.Format === 'Vinyl' ||
					record.Format === 'CD'
			},
			config: {}
		}),

	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record.Format === 'DVD' ||
					record.Format === 'Other' ||
					record.Format === 'Tapes' ||
					record.Format === 'Cassette' ||
					record.Format === 'Vinyl' ||
					record.Format === 'CD'
			},
			config: {}
		}),

	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record.Format === 'DVD' ||
					record.Format === 'Other' ||
					record.Format === 'Tapes' ||
					record.Format === 'Download' ||
					record.Format === 'Cassette' ||
					record.Format === 'Vinyl' ||
					record.Format === 'CD'
			},
			config: {}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record.Format === 'DVD' ||
					record.Format === 'Other' ||
					record.Format === 'Tapes' ||
					record.Format === 'Download' ||
					record.Format === 'Streaming' ||
					record.Format === 'Cassette' ||
					record.Format === 'Vinyl' ||
					record.Format === 'CD'
			},
			config: {}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export { description }
export default testSteps

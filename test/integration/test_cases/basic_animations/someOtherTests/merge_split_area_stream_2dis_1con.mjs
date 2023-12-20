import { data } from '../../../test_data/music_industry_history_1.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: ['Revenue', 'Format'],
					color: 'Format'
				},
				title: 'Stacked Streamgraph',
				geometry: 'area',
				align: 'center'
			},
			style: {
				plot: {
					xAxis: {
						label: {
							angle: 1.8,
							fontSize: 8.5
							//                        numberFormat: 'grouped'
						}
					}
				}
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: {
						range: {
							max: '100%'
						}
					}
				},
				title: 'Split Area Chart',
				split: true,
				align: 'min'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Revenue', 'Year'],
					y: 'Format'
				},
				title: 'Bar Chart',
				geometry: 'rectangle',
				split: false,
				align: 'min'
			},
			style: {
				plot: {
					xAxis: {
						label: {
							angle: null,
							fontSize: null
						}
					}
				}
			}
		})
]

export default testSteps

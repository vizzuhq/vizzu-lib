import { data } from '../../../test_data/music_industry_history_1.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Year',
					y: { set: ['Revenue', 'Format'], align: 'center' },
					color: 'Format'
				},
				title: 'Stacked Streamgraph',
				geometry: 'area'
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
						},
						split: true,
						align: 'none'
					}
				},
				title: 'Split Area Chart'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: ['Revenue', 'Year'],
					y: { set: 'Format', split: false }
				},
				title: 'Bar Chart',
				geometry: 'rectangle'
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

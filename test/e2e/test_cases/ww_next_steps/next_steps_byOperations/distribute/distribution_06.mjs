import { data_1974_1990 } from '../../../../test_data/IMDB_data.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_1974_1990,
			config: {
				channels: {
					x: 'Year',
					y: { set: 'Index', range: { max: '105%' } }
				},
				title: 'Dot plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: { split: true },
					y: { set: 'IMDb Rating', range: { max: '105%' } },
					noop: 'Index'
				},
				title: "Let's See the Distribution by a New Measure",
				orientation: 'vertical'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

import { data_1974_1990 } from '../../../test_data/IMDB_data.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_1974_1990,
			config: {
				channels: {
					x: 'Year',
					y: { set: 'Index', range: { max: '110%' } }
				},
				title: 'Dot plot',
				align: 'none',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: { set: 'IMDb Rating', range: { max: '110%' } },
					noop: 'Index'
				},
				title: 'Dot plot / + Continuous(?), Distribution(?)',
				align: 'none',
				orientation: 'vertical',
				split: true,
				geometry: 'circle'
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

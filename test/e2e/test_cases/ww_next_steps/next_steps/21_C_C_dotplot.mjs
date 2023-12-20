import { data_1974_1990 } from '../../../test_data/IMDB_data.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data: data_1974_1990,
			config: {
				channels: {
					x: { set: 'Year' },
					y: { set: 'Index', range: { max: '110%' } }
				},
				title: 'Dot plot',
				align: 'none',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate(
			{
				config: {
					channels: {
						x: { set: 'Year' },
						y: { set: 'IMDb Rating', range: { max: '110%' } },
						noop: { set: 'Index' }
					},
					title: 'Dot plot / + Continuous(?), Distribution(?)',
					align: 'none',
					orientation: 'vertical',
					split: true,
					geometry: 'circle'
				}
			},
			{
				delay: 1.5,
				easing: 'cubic-bezier(0.65,0,0.65,1)',
				geometry: {
					delay: 0.5,
					duration: 0.5
					//                easing: 'linear'
				},
				x: {
					delay: 0,
					duration: 0.8
					//                easing: 'ease-out'
				},
				y: {
					delay: 0.2,
					duration: 0.8
					//                easing: 'cubic-bezier(.39,0,.35,.99)'
				}
			}
		),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

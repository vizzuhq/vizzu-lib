import { data } from '../../test_data/cube_test_data.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					y: {
						set: ['Genres', 'Popularity'],
						range: { min: '110%', max: '0%' }
					},
					x: { set: ['Kinds'] },
					color: { set: ['Genres'] },
					label: { set: ['Popularity'] }
				},
				title: 'Data Cube'
			}
		})
]

export default testSteps

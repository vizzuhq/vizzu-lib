import data from '../../test_data/music_data.mjs'

const testSteps = [
	(chart) => {
		chart.feature('tooltip', true)
		return chart.animate({ data })
	},
	(chart) =>
		chart.animate([
			{
				target: {
					data: {
						filter: (input) => input.Genres === 'Pop'
					},
					config: {
						channels: {
							x: {
								set: [{ name: 'Genres' }]
							},
							y: {
								set: [{ name: 'Popularity' }, { name: 'Kinds' }]
							},
							color: {
								set: [{ name: 'Kinds' }]
							},
							label: {
								set: [{ name: 'Popularity' }]
							}
						}
					}
				}
			},
			{
				target: {
					data: {
						filter: (input) => input.Genres === 'Pop'
					},
					config: {
						channels: {
							x: {
								set: [{ name: 'Genres' }]
							},
							y: {
								set: [{ name: 'Popularity' }]
							},
							color: {
								set: [{ name: 'Kinds' }]
							},
							label: {
								set: [{ name: 'Popularity' }]
							}
						},
						geometry: 'circle'
					}
				}
			},
			{
				target: {
					data: {
						filter: null
					},
					config: {
						channels: {
							x: {
								set: []
							},
							y: {
								set: [{ name: 'Popularity' }, { name: 'Kinds' }, { name: 'Genres' }]
							},
							color: {
								set: [{ name: 'Kinds' }]
							},
							lightness: {
								set: [{ name: 'Genres' }]
							},
							label: {
								set: [{ name: 'Genres' }, { name: 'Popularity' }]
							}
						},
						geometry: 'rectangle'
					}
				}
			}
		])
]

export default testSteps

const dataLoaded = import('../assets/data/music_data.js')
const mdChartLoaded = import('../assets/javascripts/mdchart.js')

Promise.all([dataLoaded, mdChartLoaded]).then((results) => {
	const data = results[0].default
	const MdChart = results[1].default
	const mdchart = new MdChart(data, 'tutorial')

	mdchart.create([
		{
			anims: [
				(chart) => {
					return chart.animate({
						config: {
							title: 'Treemap',
							channels: {
								y: { set: ['Popularity', 'Kinds'] },
								x: { set: 'Genres' },
								color: { set: 'Kinds' },
								label: { set: 'Popularity' }
							}
						}
					})
				},
				(chart) => {
					return chart.animate({
						config: {
							channels: {
								y: {
									set: null
								},
								x: {
									set: null
								},
								size: {
									attach: ['Genres', 'Popularity']
								}
							}
						}
					})
				}
			]
		},
		{
			anims: [
				(chart) => {
					return chart.animate({
						config: {
							title: 'Bubble chart - stacked'
						}
					})
				},
				(chart) => {
					return chart.animate({
						config: {
							geometry: 'circle'
						}
					})
				}
			]
		},
		{
			anims: [
				(chart) => {
					return chart.animate({
						config: {
							title: 'Bubble chart - grouped - using the noop channel'
						}
					})
				},
				(chart) => {
					return chart.animate({
						config: {
							channels: {
								size: {
									detach: 'Genres'
								},
								noop: {
									set: 'Genres'
								}
							}
						}
					})
				}
			]
		}
	])
})

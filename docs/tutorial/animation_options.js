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
							title: 'Default options - step 1',
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
									detach: 'Kinds'
								},
								x: {
									attach: 'Kinds'
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
							title: 'Default options - step 2'
						}
					})
				},
				(chart) => {
					return chart.animate({
						config: {
							channels: {
								x: {
									detach: 'Kinds'
								},
								y: {
									attach: 'Kinds'
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
							title: 'Custom animation settings for specific groups'
						}
					})
				},
				(chart) => {
					return chart.animate(
						{
							config: {
								channels: {
									x: {
										attach: 'Kinds'
									},
									y: {
										detach: 'Kinds'
									}
								}
							}
						},
						{
							y: {
								duration: 2,
								delay: 2
							},
							style: {
								duration: 2,
								delay: 4
							}
						}
					)
				}
			]
		},
		{
			anims: [
				(chart) => {
					return chart.animate({
						config: {
							title: 'Custom options for the whole animation'
						}
					})
				},
				(chart) => {
					return chart.animate(
						{
							config: {
								channels: {
									x: {
										detach: 'Kinds'
									},
									y: {
										attach: 'Kinds'
									}
								}
							}
						},
						{
							duration: 1,
							easing: 'linear'
						}
					)
				}
			]
		},
		{
			anims: [
				(chart) => {
					return chart.animate({
						config: {
							title: 'Custom settings for both'
						}
					})
				},
				(chart) => {
					return chart.animate(
						{
							config: {
								channels: {
									x: {
										attach: 'Kinds'
									},
									y: {
										detach: 'Kinds'
									}
								}
							}
						},
						{
							duration: 1,
							easing: 'linear',
							y: {
								duration: 2,
								delay: 2
							},
							style: {
								duration: 2,
								delay: 4
							}
						}
					)
				}
			]
		},
		{
			anims: [
				(chart) => {
					return chart.animate({
						config: {
							title: 'Custom unit for duration'
						}
					})
				},
				(chart) => {
					return chart.animate(
						{
							config: {
								channels: {
									x: {
										detach: 'Kinds'
									},
									y: {
										attach: 'Kinds'
									}
								}
							}
						},
						{
							duration: '500ms'
						}
					)
				}
			]
		}
	])
})

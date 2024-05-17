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
							title: 'Using a preset'
						}
					})
				},
				(chart) => {
					return chart.animate(
						chart.constructor.presets.stackedBubble({
							size: 'Popularity',
							color: 'Kinds',
							stackedBy: 'Genres'
						})
					)
				}
			]
		},
		{
			anims: [
				(chart) => {
					return chart.animate({
						config: {
							title: 'Set sorting for a chart preset'
						}
					})
				},
				(chart) => {
					return chart.animate(
						chart.constructor.presets.radialStackedBar({
							angle: 'Popularity',
							radius: 'Genres',
							stackedBy: 'Kinds',
							sort: 'byValue'
						})
					)
				}
			]
		},
		{
			anims: [
				(chart) => {
					return chart.animate({
						config: {
							title: 'Setting style for a preset'
						}
					})
				},
				(chart) => {
					return chart.animate({
						config: chart.constructor.presets.radialBar({
							angle: 'Popularity',
							radius: 'Genres'
						}),
						style: {
							'plot.xAxis.interlacing.color': '#ffffff00'
						}
					})
				}
			]
		}
	])
})

import data from '../../test_data/music_data.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data
		}),

	(chart) =>
		chart.animate(
			chart.constructor.presets.stackedBubble({
				size: 'Popularity',
				color: 'Kinds',
				stackedBy: 'Genres',
				title: 'Using a preset'
			})
		),

	(chart) =>
		chart.animate(
			chart.constructor.presets.radialStackedBar({
				angle: 'Popularity',
				radius: 'Genres',
				stackedBy: 'Kinds',
				sort: 'byValue',
				title: 'Set sorting for a chart preset'
			})
		),

	(chart) =>
		chart.animate({
			config: chart.constructor.presets.radialBar({
				angle: 'Popularity',
				radius: 'Genres',
				title: 'Setting style for a preset'
			}),
			style: {
				'plot.xAxis.interlacing.color': '#ffffff00'
			}
		})
]

export default testSteps

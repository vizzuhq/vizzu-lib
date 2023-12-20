import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Value 5 (+/-)',
					y: 'Value 6 (+/-)',
					/* The noop channel splits the markers as all the other channels
                but will have no effect on the markers’ appearance. */
					noop: 'Joy factors',
					/* Lightness channel is used to assist the viewer
                in following the animation. */
					lightness: 'Year'
				},
				title: 'Scatter Plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year'
				},
				title: 'Dot Plot',
				legend: 'lightness'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Value 5 (+/-)',
					y: 'Value 6 (+/-)'
				},
				title: 'Scatter Plot'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					y: 'Joy factors',
					noop: null
				},
				title: 'Dot Plot'
			}
		})
]

export default testSteps

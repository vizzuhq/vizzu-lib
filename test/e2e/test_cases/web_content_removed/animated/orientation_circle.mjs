import { data } from '../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: 'Value 5 (+/-)',
					y: 'Joy factors',
					/* Lightness channel is used to assist the viewer
                in following the animation. */
					lightness: 'Joy factors',
					/* The noop channel splits the markers as all the other channels
                but will have no effect on the markers’ appearance. */
					noop: 'Year'
				},
				title: 'Dot Plot',
				geometry: 'circle'
			}
		}),

	(chart) =>
		chart.animate({
			config: {
				channels: {
					x: 'Year',
					y: 'Value 5 (+/-)',
					noop: 'Joy factors'
				},
				title: 'Dot Plot with Other Orientation'
			}
		})
]

export default testSteps

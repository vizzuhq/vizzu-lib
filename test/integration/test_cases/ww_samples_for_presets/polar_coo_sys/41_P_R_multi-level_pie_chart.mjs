import { data } from '../../../test_data/sunburst2.mjs'

const testSteps = [
	(chart) =>
		chart.animate({
			data,
			config: {
				channels: {
					x: { attach: ['Level names', 'Value'], range: { max: '100%' } },
					y: { attach: 'Levels', title: 'Levels', range: { max: '100%' } },
					label: 'Level names',
					color: 'Level names'
				},
				title: 'Fake Sunburst & Multi-level Pie Chart',
				coordSystem: 'polar'
			},
			style: {
				plot: {
					marker: {
						rectangleSpacing: '0',
						label: {
							fontSize: '0.6em',
							orientation: 'tangential',
							angle: 3.14 * -0.5
						}
					}
				}
			}
		}),
	(chart) => {
		chart.feature('tooltip', true)
		return chart
	}
]

export default testSteps

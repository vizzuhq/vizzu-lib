import { data } from '../../../../test_data/chart_types_eu.mjs'
import MarkerDropshadow from '../../../../utils/vizzu-markerdropshadow.mjs'

const testSteps = [
	async (chart) => {
		await import('https://unpkg.com/tinycolor2@1.6.0/dist/tinycolor-min.js')
		return chart
	},

	(chart) => {
		chart.feature(new MarkerDropshadow())

		return chart.animate({
			data,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				label: 'Value 2 (+)',
				title: 'Drop-shadow on the markers'
			},
			style: {
				plot: {
					marker: {
						shadowColor: '#60000060',
						shadowBlur: 9,
						shadowOffsetX: 3,
						shadowOffsetY: 3
					}
				}
			}
		})
	},
	(chart) =>
		chart.animate({
			data,
			config: {
				title: 'Smaller drop-shadow on the markers'
			},
			style: {
				plot: {
					marker: {
						shadowColor: '#00006060',
						shadowBlur: 5,
						shadowOffsetX: -2,
						shadowOffsetY: 2
					}
				}
			}
		})
]

export default testSteps

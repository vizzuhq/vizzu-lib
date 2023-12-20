import { Mouse } from '../../../../utils/mouse.mjs'

const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'index', type: 'dimension' },
				{ name: 'x', type: 'measure' },
				{ name: 'y', type: 'measure' }
			]
		}

		let index = 0

		chart.on('click', (event) => {
			const coordSystem = chart.feature.coordSystem
			const rel = coordSystem.toRelative(event.detail.position)
			chart.animate(
				{
					data: {
						records: [[index++, rel.x, rel.y]]
					}
				},
				0.1
			)
		})

		return chart.animate({
			data,
			config: {
				x: { set: 'x', range: { min: 0, max: 1 } },
				y: { set: 'y', range: { min: 0, max: 1 } },
				geometry: 'circle',
				noop: 'index',
				title: 'Click the chart to put in data points!'
			}
		})
	},
	(chart) => {
		return new Mouse(chart).click(200, 200).expectedAnimation()
	},
	(chart) => {
		return new Mouse(chart).click(300, 200).expectedAnimation()
	}
]

export default testSteps

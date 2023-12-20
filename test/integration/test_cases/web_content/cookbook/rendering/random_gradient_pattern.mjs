import { data_14 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	async (chart) => {
		await import('https://unpkg.com/tinycolor2@1.6.0/dist/tinycolor-min.js')
		return chart
	},
	(chart) => {
		let seed = 1

		const rand = () => {
			const x = Math.sin(seed++) * 10000
			return x - Math.floor(x)
		}

		const toCanvasRect = (rect) => {
			const coordSystem = chart.feature.coordSystem
			const pos = coordSystem.toCanvas({ x: rect.pos.x, y: rect.pos.y + rect.size.y })
			const pos2 = coordSystem.toCanvas({ x: rect.pos.x + rect.size.x, y: rect.pos.y })
			return { pos, size: { x: pos2.x - pos.x, y: pos2.y - pos.y } }
		}

		chart.on('draw-begin', (event) => {
			seed = 1
		})

		chart.on('plot-marker-draw', (event) => {
			const ctx = event.renderingContext
			const rect = toCanvasRect(event.detail.rect)

			const color = ctx.fillStyle
			if (!tinycolor(color).isValid()) return
			const alpha = tinycolor(color).getAlpha()

			for (let x = rect.pos.x; x <= rect.pos.x + rect.size.x; x++) {
				const y = rect.pos.y
				const yLen = rect.size.y * ((2 + rand()) / 3)

				const grad = ctx.createLinearGradient(0, y, 0, y + yLen)
				grad.addColorStop(0, tinycolor(color))
				grad.addColorStop(0.5, tinycolor(color).desaturate(25).darken(25).setAlpha(alpha))
				grad.addColorStop(1, tinycolor(color).desaturate(100).setAlpha(0))
				ctx.fillStyle = grad
				ctx.fillRect(x, y, 1, yLen)
			}

			event.preventDefault()
		})

		return chart.animate({
			data: data_14,
			config: {
				x: {
					set: 'Country_code',
					axis: false
				},
				y: {
					set: 'Value 3 (+)'
					//					range: { min: 600 },
				},
				color: 'Country_code'
			}
		})
	}
]

export default testSteps

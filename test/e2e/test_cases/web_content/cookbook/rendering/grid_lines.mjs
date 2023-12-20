import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	async (chart) => {
		await import('https://unpkg.com/tinycolor2@1.6.0/dist/tinycolor-min.js')
		return chart
	},
	(chart) => {
		const toCanvasRect = (rect) => {
			const coordSystem = chart.feature.coordSystem
			const pos = coordSystem.toCanvas({ x: rect.pos.x, y: rect.pos.y + rect.size.y })
			const pos2 = coordSystem.toCanvas({ x: rect.pos.x + rect.size.x, y: rect.pos.y })
			return { pos, size: { x: pos2.x - pos.x, y: pos2.y - pos.y } }
		}

		chart.on('plot-axis-interlacing-draw', (event) => {
			const ctx = event.renderingContext
			const rect = toCanvasRect(event.detail.rect)

			ctx.strokeStyle = '#cccccc'

			ctx.globalAlpha = tinycolor(ctx.fillStyle).getAlpha() // support fade-in

			ctx.lineWidth = 1

			const coordSystem = chart.feature.coordSystem

			if (coordSystem.toRelative({ x: 0, y: rect.pos.y }).y !== 1) {
				ctx.beginPath()
				ctx.moveTo(rect.pos.x, rect.pos.y)
				ctx.lineTo(rect.pos.x + rect.size.x, rect.pos.y)
				ctx.stroke()
			}

			if (coordSystem.toRelative({ x: 0, y: rect.pos.y + rect.size.y }).y !== 1) {
				ctx.beginPath()
				ctx.moveTo(rect.pos.x, rect.pos.y + rect.size.y)
				ctx.lineTo(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y)
				ctx.stroke()
			}

			ctx.globalAlpha = 1

			event.preventDefault()
		})

		return chart.animate({
			data,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				title: 'Axis grid lines'
			}
		})
	}
]

export default testSteps

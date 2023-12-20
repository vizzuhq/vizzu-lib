import { data_14 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) => {
		function hearth(ctx, x, y, w, h) {
			ctx.save()
			ctx.translate(x, y)
			ctx.scale(w / 69, h / 69)
			ctx.beginPath()
			ctx.moveTo(31, 17)
			ctx.bezierCurveTo(52, 0, 69, 35, 34, 62)
			ctx.bezierCurveTo(0, 46, 8, 2, 31, 17)
			ctx.fill()
			ctx.restore()
		}

		const toCanvasRect = (rect) => {
			const coordSystem = chart.feature.coordSystem
			const pos = coordSystem.toCanvas({ x: rect.pos.x, y: rect.pos.y + rect.size.y })
			const pos2 = coordSystem.toCanvas({ x: rect.pos.x + rect.size.x, y: rect.pos.y })
			return { pos, size: { x: pos2.x - pos.x, y: pos2.y - pos.y } }
		}

		chart.on('plot-marker-draw', (event) => {
			const ctx = event.renderingContext
			const rect = toCanvasRect(event.detail.rect)
			hearth(ctx, rect.pos.x, rect.pos.y, rect.size.x, rect.size.y)
			event.preventDefault()
		})

		return chart.animate({
			data: data_14,
			config: {
				x: 'Value 6 (+/-)',
				y: 'Value 5 (+/-)',
				color: 'Country',
				size: 'Value 4 (+/-)',
				geometry: 'circle',
				title: 'Custom markers'
			}
		})
	}
]

export default testSteps

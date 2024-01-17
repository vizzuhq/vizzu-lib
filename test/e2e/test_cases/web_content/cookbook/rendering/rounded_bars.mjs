import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) => {
		const toCanvasRect = (rect) => {
			const coordSystem = chart.feature.coordSystem
			const pos = coordSystem.toCanvas({ x: rect.pos.x, y: rect.pos.y + rect.size.y })
			const pos2 = coordSystem.toCanvas({ x: rect.pos.x + rect.size.x, y: rect.pos.y })
			return { pos, size: { x: pos2.x - pos.x, y: pos2.y - pos.y } }
		}

		const roundRect = (ctx, rect, radius) => {
			const x = rect.pos.x
			const y = rect.pos.y
			const w = rect.size.x
			const h = rect.size.y
			if (radius > w / 2) radius = w / 2
			if (radius > h / 2) radius = h / 2
			ctx.beginPath()
			ctx.moveTo(x + radius, y)
			ctx.lineTo(x + w - radius, y)
			ctx.quadraticCurveTo(x + w, y, x + w, y + radius)
			ctx.lineTo(x + w, y + h - radius)
			ctx.quadraticCurveTo(x + w, y + h, x + w - radius, y + h)
			ctx.lineTo(x + radius, y + h)
			ctx.quadraticCurveTo(x, y + h, x, y + h - radius)
			ctx.lineTo(x, y + radius)
			ctx.quadraticCurveTo(x, y, x + radius, y)
			ctx.closePath()
			ctx.fill()
		}

		chart.on('plot-marker-draw', (event) => {
			roundRect(event.renderingContext, toCanvasRect(event.detail.rect), 15)
			event.preventDefault()
		})

		return chart.animate({
			data,
			config: {
				x: 'Joy factors',
				y: 'Value 2 (+)',
				color: 'Joy factors',
				label: 'Value 2 (+)',
				title: 'Rounded bars'
			}
		})
	}
]

export default testSteps

import { data_4 } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	async (chart) => {
		await import('https://unpkg.com/tinycolor2@1.6.0/dist/tinycolor-min.js')
		return chart
	},
	(chart) => {
		function setDash(ctx) {
			const dashes = [
				[3, 17],
				[6, 14],
				[9, 11],
				[12, 8],
				[15, 5]
			]

			const color = ctx.fillStyle
			if (!tinycolor(color).isValid()) return

			const series = tinycolor(color).getBrightness()
			const alpha = Math.round(tinycolor(color).getAlpha() * 256).toString(16)

			ctx.setLineDash(dashes[series])

			ctx.strokeStyle = '#666666' + alpha
		}

		chart.on('legend-marker-draw', (event) => {
			const ctx = event.renderingContext

			setDash(ctx)

			const rect = event.data.rect

			ctx.beginPath()
			ctx.moveTo(rect.pos.x, rect.pos.y + rect.size.y / 2)
			ctx.lineTo(rect.pos.x + rect.size.x, rect.pos.y + rect.size.y / 2)
			ctx.stroke()

			event.preventDefault()
		})

		chart.on('plot-marker-draw', (event) => {
			const ctx = event.renderingContext

			setDash(ctx)

			const line = event.data.line

			ctx.beginPath()
			ctx.moveTo(line.begin.x, line.begin.y)
			ctx.lineTo(line.end.x, line.end.y)
			ctx.stroke()

			event.preventDefault()
		})

		return chart.animate({
			data: data_4,
			config: {
				x: 'Year',
				y: 'Value 5 (+/-)',
				color: 'Country',
				geometry: 'line',
				title: 'Custom line types'
			},
			style: {
				'plot.marker.colorPalette': '#000000 #010101 #020202 #030303 #040404'
			}
		})
	}
]

export default testSteps

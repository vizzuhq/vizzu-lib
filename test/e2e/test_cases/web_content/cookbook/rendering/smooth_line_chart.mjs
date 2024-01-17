import { data } from '../../../../test_data/chart_types_eu.mjs'

const testSteps = [
	(chart) => {
		chart.on('plot-marker-draw', (event) => {
			const ctx = event.renderingContext
			const line = event.detail.line
			ctx.beginPath()
			ctx.moveTo(line.begin.x, line.begin.y)
			ctx.bezierCurveTo(
				(line.begin.x + line.end.x) / 2,
				line.begin.y,
				(line.begin.x + line.end.x) / 2,
				line.end.y,
				line.end.x,
				line.end.y
			)
			ctx.stroke()
			event.preventDefault()
		})

		return chart.animate({
			data,
			config: {
				x: { set: ['Year'] },
				y: { set: ['Value 5 (+/-)'] },
				title: 'Smooth Line Chart',
				geometry: 'line'
			}
		})
	},
	(chart) => chart.animate({ y: 'Value 2 (+)' })
]

export default testSteps

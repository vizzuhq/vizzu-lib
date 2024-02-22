const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [15, 32, 12] }
			]
		}

		chart.on('legend-title-draw', (e) => {
			const rect = e.detail.outerRect
			const rect2 = e.detail.innerRect
			const ctx = e.renderingContext
			ctx.strokeStyle = '#00000080'
			ctx.beginPath()
			ctx.rect(rect.transform[0][2], rect.transform[1][2], rect.size.x, rect.size.y)
			ctx.stroke()
			ctx.strokeStyle = '#0000FF80'
			ctx.beginPath()
			ctx.rect(
				rect.transform[0][2] + rect2.pos.x,
				rect.transform[1][2] + rect2.pos.y,
				rect2.size.x,
				rect2.size.y
			)
			ctx.stroke()
		})

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: 'Foo',
			y: 'Bar',
			color: { set: 'Foo', title: 'ASD' },
			legend: 'color'
		})
]

export default testSteps

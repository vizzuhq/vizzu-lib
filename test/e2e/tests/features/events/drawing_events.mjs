const events = [
	'background-draw',
	'title-draw',
	'logo-draw',
	'legend-background-draw',
	'legend-title-draw',
	'legend-label-draw',
	'legend-marker-draw',
	'legend-bar-draw',
	'plot-background-draw',
	'plot-area-draw',
	'plot-marker-draw',
	'plot-marker-label-draw',
	'plot-marker-guide-draw',
	'plot-axis-draw',
	'plot-axis-title-draw',
	'plot-axis-label-draw',
	'plot-axis-tick-draw',
	'plot-axis-guide-draw',
	'plot-axis-interlacing-draw'
]

let receivedEvents = []
let isErrorLogged = false

function overlay(e, chart) {
	const coordSystem = chart.feature.coordSystem
	const ctx = e.renderingContext
	ctx.save()
	ctx.fillStyle = '#FF00000F'
	ctx.strokeStyle = '#FF0000A0'
	if (!e.detail.relative) {
		if (e.detail.outerRect) {
			const t = e.detail.outerRect.transform
			const inPos = e.detail.innerRect.pos
			const width = ctx.measureText(e.detail.text).width
			const metrics = ctx.measureText('Op')
			const height = metrics.actualBoundingBoxAscent + metrics.actualBoundingBoxDescent

			ctx.transform(t[0][0], t[1][0], t[0][1], t[1][1], t[0][2], t[1][2])

			if (e.type.includes('legend-label-draw')) {
				const orectsize = e.detail.outerRect.size
				ctx.fillStyle = '#FF00FFA0'
				ctx.fillRect(0, 0, orectsize.x, orectsize.y)
				ctx.fillStyle = '#FF00000F'
			}

			ctx.translate(inPos.x, inPos.y)
			if (e.detail.innerRect.size.x > width) {
				ctx.translate(((e.detail.innerRect.size.x - width) / 2) * (1 + e.detail.align), 0)
			}

			ctx.fillRect(0, 0, width, height)
			if (e.detail.text) {
				ctx.fillStyle = '#FF0000A0'
				ctx.textAlign = 'left'
				ctx.textBaseline = 'top'
				ctx.fillText(e.detail.text, 0, 0)
			}
		} else if (e.detail.rect && e.detail.rect.pos && e.detail.rect.size) {
			const r = e.detail.rect
			if (e.type.includes('legend-background-draw')) ctx.fillStyle = '#FFFF00A0'
			ctx.fillRect(r.pos.x, r.pos.y, r.size.x, r.size.y)
			ctx.fillStyle = '#FF0000A0'
			if (e.detail.text) ctx.fillText(e.detail.text, r.pos.x, r.pos.y, r.pos.y + r.size.y)
		} else if (e.detail.line) {
			const l = e.detail.line
			ctx.beginPath()
			ctx.moveTo(l.begin.x, l.begin.y)
			ctx.lineTo(l.end.x, l.end.y)
			ctx.stroke()
		} else console.log(e.type)
	} else {
		if (e.detail.rect && e.detail.rect.pos && e.detail.rect.size) {
			const r = e.detail.rect
			const pos = coordSystem.toCanvas(r.pos)
			const bottomRight = { x: r.pos.x + r.size.x, y: r.pos.y + r.size.y }
			const br = coordSystem.toCanvas(bottomRight)
			const size = { x: br.x - pos.x, y: br.y - pos.y }
			ctx.fillRect(pos.x, pos.y, size.x, size.y)
			ctx.fillStyle = '#FF0000A0'
			if (e.detail.text) ctx.fillText(e.detail.text, pos.x, pos.y, pos.y + size.y)
		} else if (e.detail.line) {
			const l = e.detail.line
			ctx.beginPath()
			const beg = coordSystem.toCanvas(l.begin)
			const end = coordSystem.toCanvas(l.end)
			ctx.moveTo(beg.x, beg.y)
			ctx.lineTo(end.x, end.y)
			ctx.stroke()
		} else console.log(e.type)
	}
	ctx.restore()
}

function setupEvents(chart) {
	chart.on('draw-begin', (e) => {
		receivedEvents = []
	})
	events.forEach((event) => {
		chart.on(event, (e) => {
			if (e.type === 'title-draw' && e.detail.text === '') return
			overlay(e, chart)
			receivedEvents.push(e)
			e.renderingContext.globalAlpha = 0.5
			//      e.preventDefault()
		})
	})
	chart.on('draw-complete', (e) => {
		const reference = 764260241
		receivedEvents.push(e)
		const result = JSON.stringify(receivedEvents, null, 2)
		const hash = (str) =>
			str.split('').reduce((prev, curr) => (Math.imul(31, prev) + curr.charCodeAt(0)) | 0, 0)
		if (hash(result) !== reference) {
			if (!isErrorLogged) {
				console.log('Expected hash: ' + reference)
				console.log('Actual hash: ' + hash(result))
				isErrorLogged = true
			}
			e.renderingContext.fillStyle = 'red'
			e.renderingContext.fillText('FAILED', 10, 20)
		} else {
			e.renderingContext.fillStyle = 'green'
			e.renderingContext.fillText('PASSED', 10, 20)
		}
	})
}

const data = {
	series: [
		{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
		{ name: 'Bar', values: [15, 32, 12] },
		{ name: 'Baz', values: [500, 300, 200] }
	]
}

const testSteps = [
	(chart) => {
		setupEvents(chart)
		return chart.animate({
			data,
			config: {
				color: 'Foo',
				x: { set: 'Foo', guides: true, ticks: true },
				y: { set: 'Bar', guides: true, ticks: true },
				size: 'Baz',
				label: 'Baz',
				title: 'My Chart',
				legend: 'size',
				geometry: 'circle'
			}
		}, 0)
	}
]

export default testSteps

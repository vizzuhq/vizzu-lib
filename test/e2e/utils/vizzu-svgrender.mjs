import { CRenderer } from '../../../dist/index.js'

export class SvgRenderer extends CRenderer {
	_svg = null
	_states = []

	get api() {
		return {
			getSvg: () => this._svg.svg()
		}
	}

	get hooks() {
		const hooks = {
			render: (ctx, next) => {
				ctx.renderer = this
				next()
			}
		}
		return hooks
	}

	get _defState() {
		return {
			lineWidth: 1,
			brushColor: 'rgba(0, 0, 0, 1)',
			lineColor: 'rgba(0, 0, 0, 1)',
			font: { size: 10, family: 'sans-serif' },
			path: [],
			gradient: null,
			transforms: [{ a: 1, b: 0, c: 0, d: 1, e: 0.5, f: 0.5 }],
			clip: []
		}
	}

	get _state() {
		return this._states[this._states.length - 1]
	}

	save() {
		this._states.push({
			...this._state,
			transforms: [...this._state.transforms],
			clip: [...this._state.clip]
		})
	}

	restore() {
		this._states.pop()
	}

	_transform(element) {
		this._state.transforms.forEach((t) => element.transform(t))
	}

	_draw(element) {
		element
			.fill(this._state.gradient ?? this._state.brushColor)
			.stroke({ width: this._state.lineWidth, color: this._state.lineColor })
		this._transform(element)
		this._state.clip.forEach((c) => {
			element.clipWith(c)
		})
	}

	_clip(element) {
		this._transform(element)
		this._state.clip.push(this._svg.clip().add(element))
	}

	frameBegin() {
		this._states = [this._defState]
		this._svg = window.SVG()
	}

	frameEnd() {
		this.ready(this._svg.svg())
	}

	setClipRect(x, y, sizex, sizey) {
		const rect = this._svg.rect(sizex, sizey).move(x, y)
		this._clip(rect)
	}

	setClipCircle(x, y, radius) {
		const circle = this._svg.circle(radius * 2).move(x - radius, y - radius)
		this._clip(circle)
	}

	setClipPolygon() {
		this._state.path.push(['z'])
		const path = this._svg.path(this._state.path)
		this._clip(path)
	}

	setBrushColor(r, g, b, a) {
		this._state.gradient = null
		this._state.brushColor = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
	}

	setLineColor(r, g, b, a) {
		this._state.lineColor = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
	}

	setLineWidth(width) {
		this._state.lineWidth = width
	}

	setFontStyle(font) {
		const size = font.match(/([0-9.]+)px/)[1]
		const family = font.match(/([0-9.]+)px (.+),/)[2]
		this._state.font = {
			size,
			family
		}
	}

	beginDropShadow() {}
	setDropShadowBlur(radius) {}
	setDropShadowColor(r, g, b, a) {}
	setDropShadowOffset(x, y) {}
	endDropShadow() {}

	beginPolygon() {
		this._state.path = []
	}

	addPoint(x, y) {
		this._state.path.push([this._state.path.length === 0 ? 'M' : 'L', x, y])
	}

	addBezier(c0x, c0y, c1x, c1y, x, y) {
		this._state.path.push(['C', c0x, c0y, c1x, c1y, x, y])
	}

	endPolygon() {
		this._state.path.push(['z'])
		this._draw(this._svg.path(this._state.path))
	}

	rectangle(x, y, sizex, sizey) {
		this._draw(this._svg.rect(sizex, sizey).move(x, y))
	}

	circle(x, y, radius) {
		this._draw(this._svg.circle(radius * 2).move(x - radius, y - radius))
	}

	line(x1, y1, x2, y2) {
		this._draw(this._svg.line(x1, y1, x2, y2))
	}

	drawText(x, y, sizex, sizey, text) {
		const element = this._svg
			.text(text)
			.font(this._state.font)
			.attr('dominant-baseline', 'hanging')
		this._draw(element)
	}

	setGradient(x1, y1, x2, y2, gradient) {
		this._state.gradient = this._svg.gradient('linear', (add) => {
			gradient.stops.forEach((g) => add.stop(g.offset, g.color))
		})
		this._gradient.from(x1, y1).to(x2, y2)
	}

	transform(a, b, c, d, e, f) {
		this._state.transforms.push({ a, b, c, d, e, f })
	}
}

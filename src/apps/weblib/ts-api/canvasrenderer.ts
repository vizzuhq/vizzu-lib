import { CColorGradient } from './module/ccolorgradient.js'
import { CEnv } from './module/cenv.js'
import { CPointerClosure } from './module/objregistry.js'
import { Canvas } from './module/canvas.js'
import { CCanvas } from './module/ccanvas.js'
import { HtmlCanvas } from './htmlcanvas.js'

export class CanvasRenderer extends CCanvas implements Canvas {
	private _canvas: HtmlCanvas
	private _polygonInProgress: boolean = false
	private _currentLineWidth: number = 1

	constructor(env: CEnv, getId: CPointerClosure, canvas: HtmlCanvas) {
		super(env, getId)
		this._canvas = canvas
	}

	frameBegin(): void {
		this._currentLineWidth = 1
		this._polygonInProgress = false
		this._canvas.frameBegin()
	}

	frameEnd(): void {
		this._canvas.frameEnd()
	}

	setClipRect(x: number, y: number, sizex: number, sizey: number): void {
		const dc = this._canvas.context
		dc.beginPath()
		dc.rect(x, y, sizex, sizey)
		dc.clip()
	}

	setClipCircle(x: number, y: number, radius: number): void {
		const dc = this._canvas.context
		dc.beginPath()
		dc.arc(x, y, radius, 0, 6.28318530718)
		dc.clip()
	}

	setClipPolygon(): void {
		const dc = this._canvas.context
		dc.closePath()
		dc.clip()
		this._polygonInProgress = false
	}

	setBrushColor(r: number, g: number, b: number, a: number): void {
		const dc = this._canvas.context
		dc.fillStyle = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
	}

	setLineColor(r: number, g: number, b: number, a: number): void {
		const dc = this._canvas.context
		dc.strokeStyle = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
	}

	setLineWidth(width: number): void {
		const dc = this._canvas.context
		dc.lineWidth = width
		this._currentLineWidth = width
	}

	setFontStyle(font: string): void {
		const dc = this._canvas.context
		dc.font = font
	}

	beginDropShadow(): void {}

	setDropShadowBlur(radius: number): void {
		const dc = this._canvas.context
		dc.shadowBlur = radius
	}

	setDropShadowColor(r: number, g: number, b: number, a: number): void {
		const dc = this._canvas.context
		dc.shadowColor = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
	}

	setDropShadowOffset(x: number, y: number): void {
		const dc = this._canvas.context
		dc.shadowOffsetX = x
		dc.shadowOffsetY = y
	}

	endDropShadow(): void {
		const dc = this._canvas.context
		dc.shadowBlur = 0
		dc.shadowOffsetX = 0
		dc.shadowOffsetY = 0
		dc.shadowColor = 'rgba(0, 0, 0, 0)'
	}

	beginPolygon(): void {
		const dc = this._canvas.context
		dc.beginPath()
	}

	addPoint(x: number, y: number): void {
		const dc = this._canvas.context
		if (!this._polygonInProgress) dc.moveTo(x, y)
		else dc.lineTo(x, y)
		this._polygonInProgress = true
	}

	addBezier(c0x: number, c0y: number, c1x: number, c1y: number, x: number, y: number): void {
		const dc = this._canvas.context
		dc.bezierCurveTo(c0x, c0y, c1x, c1y, x, y)
	}

	endPolygon(): void {
		const dc = this._canvas.context
		dc.closePath()
		dc.fill()
		if (this._currentLineWidth !== 0) dc.stroke()
		this._polygonInProgress = false
	}

	rectangle(x: number, y: number, sizex: number, sizey: number): void {
		const dc = this._canvas.context
		dc.beginPath()
		dc.rect(x, y, sizex, sizey)
		dc.fill()
		if (this._currentLineWidth !== 0) dc.stroke()
	}

	circle(x: number, y: number, radius: number): void {
		const dc = this._canvas.context
		dc.beginPath()
		dc.arc(x, y, radius, 0, 6.28318530718)
		dc.fill()
		if (this._currentLineWidth !== 0) dc.stroke()
	}

	line(x1: number, y1: number, x2: number, y2: number): void {
		const dc = this._canvas.context
		dc.beginPath()
		dc.moveTo(x1, y1)
		dc.lineTo(x2, y2)
		if (this._currentLineWidth !== 0) dc.stroke()
	}

	drawText(x: number, y: number, sizex: number, sizey: number, text: string): void {
		const dc = this._canvas.context
		dc.textAlign = 'left'
		dc.textBaseline = 'top'
		x = x + (sizex < 0 ? -sizex : 0)
		y = y + (sizey < 0 ? -sizey : 0)
		dc.fillText(text, x, y)
	}

	setGradient(x1: number, y1: number, x2: number, y2: number, gradient: CColorGradient): void {
		const dc = this._canvas.context
		const grd = dc.createLinearGradient(x1, y1, x2, y2)
		gradient.stops.forEach((g) => grd.addColorStop(g.offset, g.color))
		dc.fillStyle = grd
	}

	transform(a: number, b: number, c: number, d: number, e: number, f: number): void {
		const dc = this._canvas.context
		dc.transform(a, b, c, d, e, f)
	}

	save(): void {
		const dc = this._canvas.context
		dc.save()
	}

	restore(): void {
		const dc = this._canvas.context
		dc.restore()
	}
}

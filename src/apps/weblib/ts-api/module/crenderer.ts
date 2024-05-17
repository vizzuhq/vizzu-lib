import type { CCanvas } from './ccanvas.js'
import type { CColorGradient } from './ccolorgradient.js'
import type { CString, CColorGradientPtr } from '../cvizzu.types'

export abstract class CRenderer {
	canvas?: CCanvas

	abstract setFontStyle(font: string): void
	abstract drawText(x: number, y: number, sizex: number, sizey: number, text: string): void
	abstract setGradient(
		x1: number,
		y1: number,
		x2: number,
		y2: number,
		gradient: CColorGradient
	): void

	setFont(font: CString): void {
		if (this.canvas) {
			this.setFontStyle(this.canvas.getString(font))
		}
	}

	text(x: number, y: number, sizex: number, sizey: number, text: CString): void {
		if (this.canvas) {
			this.drawText(x, y, sizex, sizey, this.canvas.getString(text))
		}
	}

	setBrushGradient(
		x1: number,
		y1: number,
		x2: number,
		y2: number,
		stopCount: number,
		stops: CColorGradientPtr
	): void {
		if (this.canvas) {
			this.setGradient(x1, y1, x2, y2, this.canvas.getColorGradient(stops, stopCount))
		}
	}
}

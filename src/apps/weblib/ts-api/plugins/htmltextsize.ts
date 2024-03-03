import { Plugin } from '../plugins.js'
import { Point } from '../geom.js'
import { CTextSize } from '../module/ctextsize.js'

export class HtmlTextSize extends CTextSize implements Plugin {
	private _canvas: HTMLCanvasElement | null = null
	private _context: CanvasRenderingContext2D | null = null

	meta = { name: 'htmlTextSize' }

	getTextSize(font: string, text: string): Point {
		if (this._context === null) {
			if (this._canvas === null) {
				this._canvas = document.createElement<'canvas'>('canvas')
			}
			this._context = this._canvas.getContext('2d')
			if (!this._context) throw new Error('Failed to get 2D context')
		}
		this._context.font = font
		let metrics = this._context.measureText(text)
		const width = metrics.width
		metrics = this._context.measureText('Op')
		const height = metrics.actualBoundingBoxAscent + metrics.actualBoundingBoxDescent
		return { x: width, y: height }
	}
}

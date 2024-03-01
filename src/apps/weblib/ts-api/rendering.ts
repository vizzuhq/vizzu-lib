import { Plugin, PluginApi } from './plugins.js'
import { Module } from './module/module.js'
import { CCanvas } from './module/ccanvas.js'
import { Canvas } from './module/canvas.js'
import { CChart } from './module/cchart.js'
import { HtmlCanvas } from './htmlcanvas.js'
import { CanvasRenderer } from './canvasrenderer.js'

export interface RenderingApi extends PluginApi {
	/** Re-renders the chart. */
	update(): void
}

export class Render implements Plugin {
	private _canvas: HtmlCanvas
	private _ccanvas: CCanvas & Canvas
	private _enabled: boolean
	private _cchart: CChart

	meta = { name: 'rendering' }

	get api(): RenderingApi {
		return {
			update: (): void => {
				this.updateFrame(true)
			}
		}
	}

	enable(enabled: boolean): void {
		this._enabled = enabled
	}

	constructor(module: Module, cchart: CChart, canvas: HtmlCanvas) {
		this._canvas = canvas
		this._canvas.onchange = (): void => {
			this.updateFrame(true)
		}
		this._enabled = true
		this._cchart = cchart
		this._ccanvas = module.createCanvas<CanvasRenderer, HtmlCanvas>(CanvasRenderer, canvas)
		module.registerRenderer(this._ccanvas)
	}

	updateFrame(force: boolean = false): void {
		const size = this._canvas.calcSize()
		if (size.x >= 1 && size.y >= 1) {
			const time = performance.now()
			const renderControl = !this._enabled ? 2 : force ? 1 : 0
			this._cchart.update(this._ccanvas, size.x, size.y, time, renderControl)
		}
	}
}

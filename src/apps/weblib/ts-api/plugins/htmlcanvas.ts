import * as Geom from '../geom.js'
import * as Events from '../events.js'
import { Plugin, PluginApi, PluginHooks, PluginListeners, StartContext } from '../plugins.js'
import type { HtmlCanvasAlternative, RenderContext } from './canvasrenderer.js'

export interface CanvasOptions {
	element: HTMLElement
}

export type LazyCanvasOptions = string | HTMLElement | CanvasOptions

export type Event<T> = Events.Event<T> & {
	/** For drawing events the rendering context of the underlying 
	canvas set up for drawing the element. */
	renderingContext?: CanvasRenderingContext2D
}

export interface HtmlCanvasApi extends PluginApi {
	/** Returns the underlying canvas element. */
	get element(): HTMLCanvasElement
	/** Update the underlying canvas element. */
	set element(element: HTMLCanvasElement)
	/** Returns the actual canvas context */
	get context(): CanvasRenderingContext2D
	clientToCanvas(clientPos: Geom.Point): Geom.Point
	canvasToClient(renderPos: Geom.Point): Geom.Point
}

export class HtmlCanvas implements Plugin, HtmlCanvasAlternative {
	private _update: (force: boolean) => void = () => {}
	private _container?: HTMLElement
	private _offscreenCanvas: HTMLCanvasElement
	private _offscreenContext: CanvasRenderingContext2D
	private _mainCanvas!: HTMLCanvasElement
	private _context!: CanvasRenderingContext2D
	private _resizeObserver!: ResizeObserver
	private _resizeHandler: () => void
	private _prevUpdateHash: string = ''
	private _scaleFactor: number = 1
	private _cssWidth: number = 1
	private _cssHeight: number = 1

	meta = { name: 'htmlCanvas' }

	get api(): HtmlCanvasApi {
		const self = this
		return {
			get element(): HTMLCanvasElement { return self._mainCanvas },
			set element(newCanvasElement: HTMLCanvasElement) { self._setCanvas(newCanvasElement) },	
			context: this.context,
			clientToCanvas: this._clientToCanvas.bind(this),
			canvasToClient: this._canvasToClient.bind(this)
		}
	}

	get hooks(): PluginHooks {
		const hooks = {
			start: (ctx: StartContext, next: () => void): void => {
				this._update = ctx.update
				next()
			},
			render: (ctx: RenderContext, next: () => void): void => {
				ctx.htmlCanvas = this
				ctx.size = this._calcSize()
				next()
			}
		}
		return hooks
	}

	get listeners(): PluginListeners {
		return Object.fromEntries(
			Object.values(Events.EventType)
				.filter(
					(type: string): boolean => type.endsWith('-draw') || type.startsWith('draw-')
				)
				.map((type: string) => [type as Events.EventType, this._extendEvent.bind(this)])
		)
	}

	private _extendEvent<T>(param: Event<T>): void {
		param.renderingContext = this.context
	}

	static extractOptions(options: unknown): CanvasOptions {
		const opts =
			typeof options !== 'object' || options instanceof HTMLElement
				? { element: options }
				: options

		if (opts === null || !('element' in opts)) {
			throw new Error('element not specified')
		}

		let element = opts.element

		if (typeof element === 'string') {
			element = document.getElementById(element)
		}

		if (!(element instanceof HTMLElement)) {
			throw new Error(`Cannot find element ${opts.element} to render Vizzu!`)
		}

		return { element }
	}

	constructor(options: CanvasOptions) {
		this._offscreenCanvas = document.createElement<'canvas'>('canvas')
		const offCtx = this._offscreenCanvas.getContext('2d')
		if (!offCtx) throw Error('Cannot get rendering context of internal canvas')
		this._offscreenContext = offCtx
		this._resizeHandler = (): void => {
			this._update(true)
		}
		this._setCanvas(options.element)
		window.addEventListener('resize', this._resizeHandler)
	}

	private _setCanvas(element: HTMLElement): void {
		this._mainCanvas = this._toCanvasElement(element)
		this._prevUpdateHash = ''
		const ctx = this._mainCanvas.getContext('2d')
		if (!ctx) throw Error('Cannot get rendering context of canvas')
		this._context = ctx
		this._calcSize()
		this._resizeObserver = this._createResizeObserverFor(this._mainCanvas)
	}

	unregister(): void {
		window.removeEventListener('resize', this._resizeHandler)
		this._resizeObserver.disconnect()
		if (this._container) this._container.removeChild(this._mainCanvas)
	}

	get element(): HTMLCanvasElement {
		return this._mainCanvas
	}

	set element(element: HTMLCanvasElement) {
		this._setCanvas(element)
	}

	get context(): CanvasRenderingContext2D {
		return this._offscreenContext
	}

	private _calcSize(): Geom.Point {
		this._scaleFactor = window.devicePixelRatio
		this._cssWidth = +getComputedStyle(this._mainCanvas).width.slice(0, -2)
		this._cssHeight = +getComputedStyle(this._mainCanvas).height.slice(0, -2)
		const hash = `${this._scaleFactor}:${this._cssWidth}:${this._cssHeight}`
		if (hash !== this._prevUpdateHash) {
			this._mainCanvas.width = this._cssWidth * this._scaleFactor
			this._mainCanvas.height = this._cssHeight * this._scaleFactor
			this._offscreenCanvas.width = this._cssWidth * this._scaleFactor
			this._offscreenCanvas.height = this._cssHeight * this._scaleFactor
			this._offscreenContext.setTransform(1, 0, 0, 1, 0, 0)
			this._offscreenContext.translate(0.5, 0.5)
			this._offscreenContext.scale(this._scaleFactor, this._scaleFactor)
		}
		this._prevUpdateHash = hash
		return { x: this._cssWidth, y: this._cssHeight }
	}

	frameBegin(): void {
		this._offscreenContext.clearRect(-1, -1, this._cssWidth + 2, this._cssHeight + 2)
	}

	frameEnd(): void {
		this._context.clearRect(-1, -1, this._mainCanvas.width + 1, this._mainCanvas.height + 1)
		this._context.drawImage(this._offscreenCanvas, 0, 0)
	}

	private _clientRect(): DOMRect {
		return this._mainCanvas.getBoundingClientRect()
	}

	private _canvasScale(clientRect: DOMRect): Geom.Point {
		return {
			x: clientRect.width / (this._mainCanvas.width / this._scaleFactor),
			y: clientRect.height / (this._mainCanvas.height / this._scaleFactor)
		}
	}

	private _clientToCanvas(clientPos: Geom.Point): Geom.Point {
		const rect = this._clientRect()
		const scale = this._canvasScale(rect)
		return {
			x: (clientPos.x - rect.x) / scale.x,
			y: (clientPos.y - rect.y) / scale.y
		}
	}

	private _canvasToClient(renderPos: Geom.Point): Geom.Point {
		const rect = this._clientRect()
		const scale = this._canvasScale(rect)
		return {
			x: rect.x + renderPos.x * scale.x,
			y: rect.y + renderPos.y * scale.y
		}
	}

	private _toCanvasElement(element: HTMLElement): HTMLCanvasElement {
		if (element instanceof HTMLCanvasElement) {
			return element
		} else {
			const canvas = document.createElement<'canvas'>('canvas')
			canvas.style.width = '100%'
			canvas.style.height = '100%'
			this._container = element
			this._container.appendChild(canvas)
			return canvas
		}
	}

	private _createResizeObserverFor(canvas: HTMLCanvasElement): ResizeObserver {
		const resizeObserver = new ResizeObserver(() => {
			this._update(true)
		})
		resizeObserver.observe(canvas)
		return resizeObserver
	}
}

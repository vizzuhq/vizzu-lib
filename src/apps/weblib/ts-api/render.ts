import { Renderer } from './cvizzu.types'

import { Geom } from './types/vizzu.js'
import { CCanvas } from './module/ccanvas.js'
import { CChart } from './module/cchart.js'

export class Render implements Renderer {
  _ccanvas: CCanvas
  _cchart: CChart
  _enabled: boolean
  _log: boolean
  _polygonFirstPoint: boolean
  _offscreenCanvas: HTMLCanvasElement
  _offscreenContext: CanvasRenderingContext2D
  _mainCanvas: HTMLCanvasElement
  _context: CanvasRenderingContext2D
  _prevUpdateHash: string = ''
  _currentLineWidth: number = 1
  _scaleFactor: number = 1
  _cssWidth: number = 1
  _cssHeight: number = 1

  constructor(ccanvas: CCanvas, cchart: CChart, canvas: HTMLCanvasElement, log: boolean) {
    this._ccanvas = ccanvas
    this._enabled = true
    this._polygonFirstPoint = false
    this._offscreenCanvas = document.createElement<'canvas'>('canvas')
    const offCtx = this._offscreenCanvas.getContext('2d')
    if (!offCtx) throw Error('Cannot get rendering context of internal canvas')
    this._offscreenContext = offCtx
    this._cchart = cchart
    this._mainCanvas = canvas
    const ctx = this._mainCanvas.getContext('2d')
    if (!ctx) throw Error('Cannot get rendering context of canvas')
    this._context = ctx
    this._log = log
    this.updateCanvasSize()
  }

  canvas(): HTMLCanvasElement {
    return this._mainCanvas
  }

  dc(): CanvasRenderingContext2D {
    return this._offscreenContext
  }

  clientRect(): DOMRect {
    return this._mainCanvas.getBoundingClientRect()
  }

  frameBegin(): void {
    this._currentLineWidth = 1
    this._offscreenContext.clearRect(
      -1,
      -1,
      this._mainCanvas.width + 1,
      this._mainCanvas.height + 1
    )
  }

  frameEnd(): void {
    this._context.clearRect(-1, -1, this._mainCanvas.width + 1, this._mainCanvas.height + 1)
    this._context.drawImage(this._offscreenCanvas, 0, 0)
  }

  lineWidthNotification(width: number): void {
    this._currentLineWidth = width
  }

  noneZeroLineWidth(): boolean {
    return this._currentLineWidth !== 0
  }

  startPolygonNotification(): boolean {
    const first = this._polygonFirstPoint
    this._polygonFirstPoint = true
    return !first
  }

  endPolygonNotification(): void {
    this._polygonFirstPoint = false
  }

  updateCanvasSize(): void {
    this._scaleFactor = window.devicePixelRatio
    this._cssWidth = +getComputedStyle(this._mainCanvas).width.slice(0, -2)
    this._cssHeight = +getComputedStyle(this._mainCanvas).height.slice(0, -2)
    const hash = `${this._scaleFactor}:${this._cssWidth}:${this._cssHeight}`
    if (hash !== this._prevUpdateHash) {
      this._mainCanvas.width = this._cssWidth * this._scaleFactor
      this._mainCanvas.height = this._cssHeight * this._scaleFactor
      this._offscreenCanvas.width = this._cssWidth * this._scaleFactor
      this._offscreenCanvas.height = this._cssHeight * this._scaleFactor
      this._offscreenContext.translate(0.5, 0.5)
      this._offscreenContext.scale(this._scaleFactor, this._scaleFactor)
    }
    this._prevUpdateHash = hash
  }

  updateFrame(force: boolean = false): void {
    const start = performance.now()
    this.updateCanvasSize()
    if (this._mainCanvas.width > 0 && this._mainCanvas.height > 0) {
      const renderControl = !this._enabled ? 2 : force ? 1 : 0
      this._cchart.update(this._ccanvas, this._cssWidth, this._cssHeight, renderControl)
    }
    const time = performance.now() - start
    if (this._log && time > 1) {
      console.log('Render.updateFrame: ' + time.toFixed(2) + 'ms')
    }
  }

  clientToRenderCoor(clientPos: Geom.Point): Geom.Point {
    const rect = this.clientRect()
    const scaleX = rect.width / (this._mainCanvas.width / this._scaleFactor)
    const scaleY = rect.height / (this._mainCanvas.height / this._scaleFactor)
    return {
      x: (clientPos.x - rect.x) / scaleX,
      y: (clientPos.y - rect.y) / scaleY
    }
  }
}

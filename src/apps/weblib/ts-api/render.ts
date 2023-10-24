import { Renderer } from './cvizzu.types'
import { Point } from './geom.js'
import { Plugin, PluginApi } from './plugins.js'

import { Module } from './module/module.js'
import { CCanvas } from './module/ccanvas.js'
import { CChart } from './module/cchart.js'

export interface RenderingApi extends PluginApi {
  clientToRenderCoor(clientPos: Point): Point
}

export class Render implements Plugin, Renderer {
  private _ccanvas: CCanvas
  private _enabled: boolean
  private _cchart: CChart
  private _log: boolean
  private _polygonFirstPoint: boolean
  private _offscreenCanvas: HTMLCanvasElement
  private _offscreenContext: CanvasRenderingContext2D
  private _mainCanvas: HTMLCanvasElement
  private _context: CanvasRenderingContext2D
  private _prevUpdateHash: string = ''
  private _currentLineWidth: number = 1
  private _scaleFactor: number = 1
  private _cssWidth: number = 1
  private _cssHeight: number = 1

  meta = { name: 'rendering' }

  get api(): RenderingApi {
    return {
      clientToRenderCoor: (clientPos: Point): Point => {
        return this.clientToRenderCoor(clientPos)
      }
    }
  }

  enable(enabled: boolean): void {
    this._enabled = enabled
  }

  constructor(module: Module, cchart: CChart, canvas: HTMLCanvasElement, log: boolean) {
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
    this._ccanvas = module.createCanvas()
    module.registerRenderer(this._ccanvas, this)
    this._updateCanvasSize()
  }

  canvas(): HTMLCanvasElement {
    return this._mainCanvas
  }

  dc(): CanvasRenderingContext2D {
    return this._offscreenContext
  }

  private _clientRect(): DOMRect {
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

  private _updateCanvasSize(): void {
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
    this._updateCanvasSize()
    if (this._mainCanvas.width > 0 && this._mainCanvas.height > 0) {
      const renderControl = !this._enabled ? 2 : force ? 1 : 0
      this._cchart.update(this._ccanvas, this._cssWidth, this._cssHeight, renderControl)
    }
    const time = performance.now() - start
    if (this._log && time > 1) {
      console.log('Render.updateFrame: ' + time.toFixed(2) + 'ms')
    }
  }

  clientToRenderCoor(clientPos: Point): Point {
    const rect = this._clientRect()
    const scaleX = rect.width / (this._mainCanvas.width / this._scaleFactor)
    const scaleY = rect.height / (this._mainCanvas.height / this._scaleFactor)
    return {
      x: (clientPos.x - rect.x) / scaleX,
      y: (clientPos.y - rect.y) / scaleY
    }
  }
}

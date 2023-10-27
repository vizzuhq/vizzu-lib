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
  private _polygonInProgress: boolean
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
    this._polygonInProgress = false
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

  setCursor(name: string): void {
    this.canvas().style.cursor = name
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

  setClipRect(x: number, y: number, sizex: number, sizey: number): void {
    const dc = this.dc()
    dc.beginPath()
    dc.rect(x, y, sizex, sizey)
    dc.clip()
  }

  setClipCircle(x: number, y: number, radius: number): void {
    const dc = this.dc()
    dc.beginPath()
    dc.arc(x, y, radius, 0, 6.28318530718)
    dc.clip()
  }

  setClipPolygon(): void {
    const dc = this.dc()
    dc.closePath()
    dc.clip()
    this._polygonInProgress = false
  }

  setBrushColor(r: number, g: number, b: number, a: number): void {
    const dc = this.dc()
    dc.fillStyle = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
  }

  setLineColor(r: number, g: number, b: number, a: number): void {
    const dc = this.dc()
    dc.strokeStyle = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
  }

  setLineWidth(width: number): void {
    const dc = this.dc()
    dc.lineWidth = width
    this._currentLineWidth = width
  }

  setFont(font: string): void {
    const dc = this.dc()
    dc.font = font
  }

  setDropShadowBlur(radius: number): void {
    const dc = this.dc()
    dc.shadowBlur = radius
  }

  setDropShadowColor(r: number, g: number, b: number, a: number): void {
    const dc = this.dc()
    dc.shadowColor = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
  }

  setDropShadowOffset(x: number, y: number): void {
    const dc = this.dc()
    dc.shadowOffsetX = x
    dc.shadowOffsetY = y
  }

  endDropShadow(): void {
    const dc = this.dc()
    dc.shadowBlur = 0
    dc.shadowOffsetX = 0
    dc.shadowOffsetY = 0
    dc.shadowColor = 'rgba(0, 0, 0, 0)'
  }

  beginPolygon(): void {
    const dc = this.dc()
    dc.beginPath()
  }

  addPoint(x: number, y: number): void {
    const dc = this.dc()
    if (!this._polygonInProgress) dc.moveTo(x, y)
    else dc.lineTo(x, y)
    this._polygonInProgress = true
  }

  addBezier(c0x: number, c0y: number, c1x: number, c1y: number, x: number, y: number): void {
    const dc = this.dc()
    dc.bezierCurveTo(c0x, c0y, c1x, c1y, x, y)
  }

  endPolygon(): void {
    const dc = this.dc()
    dc.closePath()
    dc.fill()
    if (this._currentLineWidth !== 0) dc.stroke()
    this._polygonInProgress = false
  }

  rectangle(x: number, y: number, sizex: number, sizey: number): void {
    const dc = this.dc()
    dc.beginPath()
    dc.rect(x, y, sizex, sizey)
    dc.fill()
    if (this._currentLineWidth !== 0) dc.stroke()
  }

  circle(x: number, y: number, radius: number): void {
    const dc = this.dc()
    dc.beginPath()
    dc.arc(x, y, radius, 0, 6.28318530718)
    dc.fill()
    if (this._currentLineWidth !== 0) dc.stroke()
  }

  line(x1: number, y1: number, x2: number, y2: number): void {
    const dc = this.dc()
    dc.beginPath()
    dc.moveTo(x1, y1)
    dc.lineTo(x2, y2)
    if (this._currentLineWidth !== 0) dc.stroke()
  }

  textBoundary(text: string): { width: number; height: number } {
    const dc = this.dc()
    let metrics = dc.measureText(text)
    const width = metrics.width
    metrics = dc.measureText('Op')
    const height = metrics.actualBoundingBoxAscent + metrics.actualBoundingBoxDescent
    return { width, height }
  }

  text(x: number, y: number, sizex: number, sizey: number, text: string): void {
    const dc = this.dc()
    dc.textAlign = 'left'
    dc.textBaseline = 'top'
    x = x + (sizex < 0 ? -sizex : 0)
    y = y + (sizey < 0 ? -sizey : 0)
    dc.fillText(text, x, y)
  }

  setBrushGradient(
    x1: number,
    y1: number,
    x2: number,
    y2: number,
    gradient: { offset: number; color: string }[]
  ): void {
    const dc = this.dc()
    const grd = dc.createLinearGradient(x1, y1, x2, y2)
    gradient.forEach((g) => grd.addColorStop(g.offset, g.color))
    dc.fillStyle = grd
  }

  transform(a: number, b: number, c: number, d: number, e: number, f: number): void {
    const dc = this.dc()
    dc.transform(a, b, c, d, e, f)
  }

  save(): void {
    const dc = this.dc()
    dc.save()
  }

  restore(): void {
    const dc = this.dc()
    dc.restore()
  }
}

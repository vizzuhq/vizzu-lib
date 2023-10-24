export class Render {
  get api() {
    return {
      clientToRenderCoor: (clientPos) => {
        return this.clientToRenderCoor(clientPos)
      }
    }
  }
  enable(enabled) {
    this._enabled = enabled
  }
  constructor(module, cchart, canvas, log) {
    this._prevUpdateHash = ''
    this._currentLineWidth = 1
    this._scaleFactor = 1
    this._cssWidth = 1
    this._cssHeight = 1
    this.meta = { name: 'rendering' }
    this._enabled = true
    this._polygonFirstPoint = false
    this._offscreenCanvas = document.createElement('canvas')
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
  canvas() {
    return this._mainCanvas
  }
  dc() {
    return this._offscreenContext
  }
  _clientRect() {
    return this._mainCanvas.getBoundingClientRect()
  }
  frameBegin() {
    this._currentLineWidth = 1
    this._offscreenContext.clearRect(
      -1,
      -1,
      this._mainCanvas.width + 1,
      this._mainCanvas.height + 1
    )
  }
  frameEnd() {
    this._context.clearRect(-1, -1, this._mainCanvas.width + 1, this._mainCanvas.height + 1)
    this._context.drawImage(this._offscreenCanvas, 0, 0)
  }
  lineWidthNotification(width) {
    this._currentLineWidth = width
  }
  noneZeroLineWidth() {
    return this._currentLineWidth !== 0
  }
  startPolygonNotification() {
    const first = this._polygonFirstPoint
    this._polygonFirstPoint = true
    return !first
  }
  endPolygonNotification() {
    this._polygonFirstPoint = false
  }
  _updateCanvasSize() {
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
  updateFrame(force = false) {
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
  clientToRenderCoor(clientPos) {
    const rect = this._clientRect()
    const scaleX = rect.width / (this._mainCanvas.width / this._scaleFactor)
    const scaleY = rect.height / (this._mainCanvas.height / this._scaleFactor)
    return {
      x: (clientPos.x - rect.x) / scaleX,
      y: (clientPos.y - rect.y) / scaleY
    }
  }
}

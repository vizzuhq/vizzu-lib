export class Rendering {
  meta = { name: 'rendering' }

  register(chart) {
    this.chart = chart
  }

  enable(enabled) {
    this.chart._validateModule()
    this.chart.render.enabled = enabled
  }
}

export class Render {
  init(ccanvas, update, canvas, log) {
    this.ccanvas = ccanvas
    this.enabled = true
    this.polygonFirstPoint = false
    this.offscreenCanvas = document.createElement('CANVAS')
    this.offscreenContext = this.offscreenCanvas.getContext('2d')
    this.update = update
    this.mainCanvas = canvas
    this.context = canvas.getContext('2d')
    this.log = log
    this.updateCanvasSize()
    this.prevUpdateHash = ''
  }

  canvas() {
    return this.mainCanvas
  }

  dc() {
    return this.offscreenContext
  }

  clientRect() {
    return this.mainCanvas.getBoundingClientRect()
  }

  frameBegin() {
    this.currentLineWidth = 1
    this.offscreenContext.clearRect(-1, -1, this.mainCanvas.width + 1, this.mainCanvas.height + 1)
  }

  frameEnd() {
    this.context.clearRect(-1, -1, this.mainCanvas.width + 1, this.mainCanvas.height + 1)
    this.context.drawImage(this.offscreenCanvas, 0, 0)
  }

  lineWidthNotification(width) {
    this.currentLineWidth = width
  }

  noneZeroLineWidth() {
    return this.currentLineWidth !== 0
  }

  startPolygonNotification() {
    const first = this.polygonFirstPoint
    this.polygonFirstPoint = true
    return !first
  }

  endPolygonNotification() {
    this.polygonFirstPoint = false
  }

  updateCanvasSize() {
    this.scaleFactor = window.devicePixelRatio
    this.cssWidth = +getComputedStyle(this.mainCanvas).width.slice(0, -2)
    this.cssHeight = +getComputedStyle(this.mainCanvas).height.slice(0, -2)
    const hash = `${this.scaleFactor}:${this.cssWidth}:${this.cssHeight}`
    if (hash !== this.prevUpdateHash) {
      this.mainCanvas.width = this.cssWidth * this.scaleFactor
      this.mainCanvas.height = this.cssHeight * this.scaleFactor
      this.offscreenCanvas.width = this.cssWidth * this.scaleFactor
      this.offscreenCanvas.height = this.cssHeight * this.scaleFactor
      this.offscreenContext.translate(0.5, 0.5)
      this.offscreenContext.scale(this.scaleFactor, this.scaleFactor)
    }
    this.prevUpdateHash = hash
  }

  updateFrame(force = false) {
    const start = performance.now()
    this.updateCanvasSize()
    if (this.mainCanvas.width > 0 && this.mainCanvas.height > 0) {
      const renderControl = !this.enabled ? 2 : force ? 1 : 0
      this.update(this.ccanvas.id, this.cssWidth, this.cssHeight, renderControl)
    }
    const time = performance.now() - start
    if (this.log && time > 1) {
      console.log('Render.updateFrame: ' + time.toFixed(2) + 'ms')
    }
  }

  clientToRenderCoor(clientPos) {
    const rect = this.clientRect()
    const scaleX = rect.width / (this.mainCanvas.width / this.scaleFactor)
    const scaleY = rect.height / (this.mainCanvas.height / this.scaleFactor)
    return {
      x: (clientPos.x - rect.x) / scaleX,
      y: (clientPos.y - rect.y) / scaleY
    }
  }
}

export default class Render {
  init(update, canvas, log) {
    this.polygonFirstPoint = false;
    // todo: implement alternate solution for nodejs
    this.offscreenCanvas = document.createElement("CANVAS");
    this.offscreenContext = this.offscreenCanvas.getContext("2d");
    this.update = update;
    this.mainCanvas = canvas;
    this.mainCanvas.font = "Roboto Condensed";
    this.mainCanvas.textAlign = "left";
    this.mainCanvas.textBaseline = "top";
    this.context = canvas.getContext("2d");
    this.log = log;
    this.updateCanvasSize();
    this.prevUpdateHash = "";
  }

  canvas() {
    return this.mainCanvas;
  }

  dc() {
    return this.offscreenContext;
  }

  clientRect() {
    return this.mainCanvas.getBoundingClientRect();
  }

  frameBegin() {
    this.currentLineWidth = 1;
    this.offscreenContext.clearRect(
      0,
      0,
      this.mainCanvas.width,
      this.mainCanvas.height
    );
  }

  frameEnd() {
    this.context.clearRect(0, 0, this.mainCanvas.width, this.mainCanvas.height);
    this.context.drawImage(this.offscreenCanvas, 0, 0);
  }

  lineWidthNotification(width) {
    this.currentLineWidth = width;
  }

  noneZeroLineWidth() {
    return this.currentLineWidth !== 0;
  }

  startPolygonNotification() {
    var first = this.polygonFirstPoint;
    this.polygonFirstPoint = true;
    return !first;
  }

  endPolygonNotification() {
    this.polygonFirstPoint = false;
  }

  updateCanvasSize() {
    this.scaleFactor = window.devicePixelRatio;
    this.cssWidth = +getComputedStyle(this.mainCanvas).width.slice(0, -2);
    this.cssHeight = +getComputedStyle(this.mainCanvas).height.slice(0, -2);
    let hash = `${this.scaleFactor}:${this.cssWidth}:${this.cssHeight}`;
    if (hash !== this.prevUpdateHash) {
      this.mainCanvas.width = this.cssWidth * this.scaleFactor;
      this.mainCanvas.height = this.cssHeight * this.scaleFactor;
      this.offscreenCanvas.width = this.cssWidth * this.scaleFactor;
      this.offscreenCanvas.height = this.cssHeight * this.scaleFactor;
      this.offscreenContext.translate(0.5, 0.5);
      this.offscreenContext.scale(this.scaleFactor, this.scaleFactor);
    }
    this.prevUpdateHash = hash;
  }

  updateFrame(force) {
    var start = performance.now();
    this.updateCanvasSize();
    if (this.mainCanvas.width > 0 && this.mainCanvas.height > 0) {
      this.update(this.scaleFactor, this.cssWidth, this.cssHeight, force);
    }
    var time = performance.now() - start;
    if (this.log && time > 1) {
      console.log("Render.updateFrame: " + time.toFixed(2) + "ms");
    }
  }
}

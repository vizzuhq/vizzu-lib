mergeInto(LibraryManager.library, {
  openUrl: function (url) {
    window.open(UTF8ToString(url), '_blank')
  },
  setCursor: function (cName) {
    const [render] = Object.values(Module.renders)
    render.canvas().style.cursor = UTF8ToString(cName)
  },
  callLater: function(func, obj, later) {
		setTimeout(() => Module._call(func)(obj), Math.max(later, 20));
	},
	canvas_frameBegin: function(canvas) {
		Module.renders[canvas].frameBegin()
  },
  canvas_frameEnd: function (canvas) {
    Module.renders[canvas].frameEnd()
  },
  canvas_setClipRect: function (canvas, x, y, sizex, sizey) {
    var dc = Module.renders[canvas].dc()
    dc.beginPath()
    dc.rect(x, y, sizex, sizey)
    dc.clip()
  },
  canvas_setClipCircle: function (canvas, x, y, radius) {
    var dc = Module.renders[canvas].dc()
    dc.beginPath()
    dc.arc(x, y, radius, 0, 6.28318530718)
    dc.clip()
  },
  canvas_setClipPolygon: function (canvas) {
    const canv = Module.renders[canvas]
    var dc = canv.dc()
    dc.closePath()
    dc.clip()
    canv.endPolygonNotification()
  },
  canvas_setBrushColor: function (canvas, r, g, b, a) {
    var dc = Module.renders[canvas].dc()
    dc.fillStyle = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
  },
  canvas_setLineColor: function (canvas, r, g, b, a) {
    var dc = Module.renders[canvas].dc()
    dc.strokeStyle = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
  },
  canvas_setLineWidth: function (canvas, width) {
    const canv = Module.renders[canvas]
    var dc = canv.dc()
    dc.lineWidth = width
    canv.lineWidthNotification(width)
  },
  canvas_setFont: function (canvas, font) {
    var dc = Module.renders[canvas].dc()
    dc.font = UTF8ToString(font)
  },
  canvas_beginDropShadow: function (canvas) {},
  canvas_setDropShadowBlur: function (canvas, radius) {
    let dc = Module.renders[canvas].dc()
    dc.shadowBlur = radius
  },
  canvas_setDropShadowColor: function (canvas, r, g, b, a) {
    let dc = Module.renders[canvas].dc()
    dc.shadowColor = 'rgba(' + r * 255 + ',' + g * 255 + ',' + b * 255 + ',' + a + ')'
  },
  canvas_setDropShadowOffset: function (canvas, x, y) {
    let dc = Module.renders[canvas].dc()
    dc.shadowOffsetX = x
    dc.shadowOffsetY = y
  },
  canvas_endDropShadow: function (canvas) {
    let dc = Module.renders[canvas].dc()
    dc.shadowBlur = 0
    dc.shadowOffsetX = 0
    dc.shadowOffsetY = 0
    dc.shadowColor = 'rgba(0, 0, 0, 0)'
  },
  canvas_beginPolygon: function (canvas) {
    var dc = Module.renders[canvas].dc()
    dc.beginPath()
  },
  canvas_addPoint: function (canvas, x, y) {
    const canv = Module.renders[canvas]
    var dc = canv.dc()
    if (canv.startPolygonNotification()) dc.moveTo(x, y)
    else dc.lineTo(x, y)
  },
  canvas_addBezier: function (canvas, c0x, c0y, c1x, c1y, x, y) {
    var dc = Module.renders[canvas].dc()
    dc.bezierCurveTo(c0x, c0y, c1x, c1y, x, y)
  },
  canvas_endPolygon: function (canvas) {
    const canv = Module.renders[canvas]
    var dc = canv.dc()
    dc.closePath()
    dc.fill()
    if (canv.noneZeroLineWidth()) dc.stroke()
    canv.endPolygonNotification()
  },
  canvas_rectangle: function (canvas, x, y, sizex, sizey) {
    const canv = Module.renders[canvas]
    var dc = canv.dc()
    dc.beginPath()
    dc.rect(x, y, sizex, sizey)
    dc.fill()
    if (canv.noneZeroLineWidth()) dc.stroke()
  },
  canvas_circle: function (canvas, x, y, radius) {
    const canv = Module.renders[canvas]
    var dc = canv.dc()
    dc.beginPath()
    dc.arc(x, y, radius, 0, 6.28318530718)
    dc.fill()
    if (canv.noneZeroLineWidth()) dc.stroke()
  },
  canvas_line: function (canvas, x1, y1, x2, y2) {
    const canv = Module.renders[canvas]
    var dc = canv.dc()
    dc.beginPath()
    dc.moveTo(x1, y1)
    dc.lineTo(x2, y2)
    if (canv.noneZeroLineWidth()) dc.stroke()
  },
  canvas_textBoundary: function (canvas, text, sizeX, sizeY) {
    var dc = Module.renders[canvas].dc()
    var metrics = dc.measureText(UTF8ToString(text))
    var width = metrics.width
    metrics = dc.measureText('Op')
    var height = metrics.actualBoundingBoxAscent + metrics.actualBoundingBoxDescent
    setValue(sizeX, width, 'double')
    setValue(sizeY, height, 'double')
  },
  canvas_text: function (canvas, x, y, sizex, sizey, text) {
    var dc = Module.renders[canvas].dc()
    dc.textAlign = 'left'
    dc.textBaseline = 'top'
    x = x + (sizex < 0 ? -sizex : 0)
    y = y + (sizey < 0 ? -sizey : 0)
    dc.fillText(UTF8ToString(text), x, y)
  },
  canvas_setBrushGradient: function (canvas, x1, y1, x2, y2, stopsCount, stops) {
    var dc = Module.renders[canvas].dc()
    var grd = dc.createLinearGradient(x1, y1, x2, y2)
    for (var i = 0; i < stopsCount * 40; i += 40) {
      grd.addColorStop(
        getValue(stops + i, 'double'),
        'rgba(' +
          getValue(stops + i + 8, 'double') * 255 +
          ',' +
          getValue(stops + i + 16, 'double') * 255 +
          ',' +
          getValue(stops + i + 24, 'double') * 255 +
          ',' +
          getValue(stops + i + 32, 'double') +
          ')'
      )
    }
    dc.fillStyle = grd
  },
  canvas_transform: function (canvas, a, b, c, d, e, f) {
    var dc = Module.renders[canvas].dc()
    dc.transform(a, b, c, d, e, f)
  },
  canvas_save: function (canvas) {
    var dc = Module.renders[canvas].dc()
    dc.save()
  },
  canvas_restore: function (canvas) {
    var dc = Module.renders[canvas].dc()
    dc.restore()
  }
})

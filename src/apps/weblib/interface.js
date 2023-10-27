/// <reference path="./ts-api/cvizzu.types.d.ts" />
mergeInto(LibraryManager.library, {
  openUrl: function (url) {
    window.open(UTF8ToString(url), '_blank')
  },
  callLater: function (func, obj, later) {
    setTimeout(() => Module.callback(func, obj), Math.max(later, 0))
  },
  setCursor: function (canvas, cName) {
    Module.renders[canvas].setCursor(UTF8ToString(cName))
  },
  canvas_frameBegin: function (canvas) {
    Module.renders[canvas].frameBegin()
  },
  canvas_frameEnd: function (canvas) {
    Module.renders[canvas].frameEnd()
  },
  canvas_setClipRect: function (canvas, x, y, sizex, sizey) {
    Module.renders[canvas].setClipRect(x, y, sizex, sizey)
  },
  canvas_setClipCircle: function (canvas, x, y, radius) {
    Module.renders[canvas].setClipCircle(x, y, radius)
  },
  canvas_setClipPolygon: function (canvas) {
    Module.renders[canvas].setClipPolygon()
  },
  canvas_setBrushColor: function (canvas, r, g, b, a) {
    Module.renders[canvas].setBrushColor(r, g, b, a)
  },
  canvas_setLineColor: function (canvas, r, g, b, a) {
    Module.renders[canvas].setLineColor(r, g, b, a)
  },
  canvas_setLineWidth: function (canvas, width) {
    Module.renders[canvas].setLineWidth(width)
  },
  canvas_setFont: function (canvas, font) {
    Module.renders[canvas].setFont(UTF8ToString(font))
  },
  canvas_beginDropShadow: function (_canvas) {},
  canvas_setDropShadowBlur: function (canvas, radius) {
    Module.renders[canvas].setDropShadowBlur(radius)
  },
  canvas_setDropShadowColor: function (canvas, r, g, b, a) {
    Module.renders[canvas].setDropShadowColor(r, g, b, a)
  },
  canvas_setDropShadowOffset: function (canvas, x, y) {
    Module.renders[canvas].setDropShadowOffset(x, y)
  },
  canvas_endDropShadow: function (canvas) {
    Module.renders[canvas].endDropShadow()
  },
  canvas_beginPolygon: function (canvas) {
    Module.renders[canvas].beginPolygon()
  },
  canvas_addPoint: function (canvas, x, y) {
    Module.renders[canvas].addPoint(x, y)
  },
  canvas_addBezier: function (canvas, c0x, c0y, c1x, c1y, x, y) {
    Module.renders[canvas].addBezier(c0x, c0y, c1x, c1y, x, y)
  },
  canvas_endPolygon: function (canvas) {
    Module.renders[canvas].endPolygon()
  },
  canvas_rectangle: function (canvas, x, y, sizex, sizey) {
    Module.renders[canvas].rectangle(x, y, sizex, sizey)
  },
  canvas_circle: function (canvas, x, y, radius) {
    Module.renders[canvas].circle(x, y, radius)
  },
  canvas_line: function (canvas, x1, y1, x2, y2) {
    Module.renders[canvas].line(x1, y1, x2, y2)
  },
  canvas_textBoundary: function (canvas, text, sizeX, sizeY) {
    const size = Module.renders[canvas].textBoundary(UTF8ToString(text))
    setValue(sizeX, size.width, 'double')
    setValue(sizeY, size.height, 'double')
  },
  canvas_text: function (canvas, x, y, sizex, sizey, text) {
    Module.renders[canvas].text(x, y, sizex, sizey, UTF8ToString(text))
  },
  canvas_setBrushGradient: function (canvas, x1, y1, x2, y2, stopsCount, stops) {
    const grd = []
    for (let i = 0; i < stopsCount * 40; i += 40) {
      grd.push({
        offset: getValue(stops + i, 'double'),
        color:
          'rgba(' +
          getValue(stops + i + 8, 'double') * 255 +
          ',' +
          getValue(stops + i + 16, 'double') * 255 +
          ',' +
          getValue(stops + i + 24, 'double') * 255 +
          ',' +
          getValue(stops + i + 32, 'double') +
          ')'
      })
    }
    Module.renders[canvas].setBrushGradient(x1, y1, x2, y2, grd)
  },
  canvas_transform: function (canvas, a, b, c, d, e, f) {
    Module.renders[canvas].transform(a, b, c, d, e, f)
  },
  canvas_save: function (canvas) {
    Module.renders[canvas].save()
  },
  canvas_restore: function (canvas) {
    Module.renders[canvas].restore()
  }
})

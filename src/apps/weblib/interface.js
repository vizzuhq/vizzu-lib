function Method(name, ...args) {
  const argList = [...args].join(', ')
  // eslint-disable-next-line no-new-func
  return new Function('canvas', ...args, `Module.renders[canvas].${name}(${argList})`)
}

mergeInto(LibraryManager.library, {
  openUrl: function (url) {
    window.open(UTF8ToString(url), '_blank')
  },
  callLater: function (func, obj, later) {
    setTimeout(() => Module.callback(func, obj), Math.max(later, 0))
  },
  setCursor: function (canvas, cName) {
    Module.renders[canvas].setCursor(cName)
  },
  canvas_frameBegin: Method('frameBegin'),
  canvas_frameEnd: Method('frameEnd'),
  canvas_setClipRect: Method('setClipRect', 'x', 'y', 'sizex', 'sizey'),
  canvas_setClipCircle: Method('setClipCircle', 'x', 'y', 'radius'),
  canvas_setClipPolygon: Method('setClipPolygon'),
  canvas_setBrushColor: Method('setBrushColor', 'r', 'g', 'b', 'a'),
  canvas_setLineColor: Method('setLineColor', 'r', 'g', 'b', 'a'),
  canvas_setLineWidth: Method('setLineWidth', 'width'),
  canvas_setFont: Method('setFont', 'font'),
  canvas_beginDropShadow: Method('beginDropShadow'),
  canvas_setDropShadowBlur: Method('setDropShadowBlur', 'radius'),
  canvas_setDropShadowColor: Method('setDropShadowColor', 'r', 'g', 'b', 'a'),
  canvas_setDropShadowOffset: Method('setDropShadowOffset', 'x', 'y'),
  canvas_endDropShadow: Method('endDropShadow'),
  canvas_beginPolygon: Method('beginPolygon'),
  canvas_addPoint: Method('addPoint', 'x', 'y'),
  canvas_addBezier: Method('addBezier', 'c0x', 'c0y', 'c1x', 'c1y', 'x', 'y'),
  canvas_endPolygon: Method('endPolygon'),
  canvas_rectangle: Method('rectangle', 'x', 'y', 'sizex', 'sizey'),
  canvas_circle: Method('circle', 'x', 'y', 'radius'),
  canvas_line: Method('line', 'x1', 'y1', 'x2', 'y2'),
  canvas_textBoundary: Method('textBoundary', 'text', 'sizex', 'sizey'),
  canvas_text: Method('text', 'x', 'y', 'sizex', 'sizey', 'text'),
  canvas_setBrushGradient: Method('setBrushGradient', 'x1', 'y1', 'x2', 'y2', 'stopCount', 'stops'),
  canvas_transform: Method('transform', 'a', 'b', 'c', 'd', 'e', 'f'),
  canvas_save: Method('save'),
  canvas_restore: Method('restore')
})

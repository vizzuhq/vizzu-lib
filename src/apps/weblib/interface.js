mergeInto(LibraryManager.library, {
	chart_openUrl: function (chart, url) {
		Module.charts[chart].openUrl(url)
	},
	chart_doChange: function (chart) {
		Module.charts[chart].doChange()
	},
	textBoundary: function (font, text, sizeX, sizeY) {
		const dc = Module.measureCanvas
		dc.font = UTF8ToString(font)
		let metrics = dc.measureText(UTF8ToString(text))
		const width = metrics.width
		metrics = dc.measureText('Op')
		const height = metrics.actualBoundingBoxAscent + metrics.actualBoundingBoxDescent
		setValue(sizeX, width, 'double')
		setValue(sizeY, height, 'double')
	}
})

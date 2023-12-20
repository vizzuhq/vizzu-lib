mergeInto(LibraryManager.library, {
	openUrl: function (url) {
		window.open(UTF8ToString(url), '_blank')
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

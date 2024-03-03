mergeInto(LibraryManager.library, {
	openUrl: function (url) {
		window.open(UTF8ToString(url), '_blank')
	},
	textBoundary: function (font, text, sizeX, sizeY) {
		Module.textSize.setSize(font, text, sizeX, sizeY)
	}
})

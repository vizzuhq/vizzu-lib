try {
	chart.off('logo-draw', assets.eventHandler)
} catch (error) {
	if (!error.toString().includes('unknown event handler')) {
		throw error
	}
}
chart = chart.animate({
	config: {
		title: 'Prevent default behavior'
	}
})

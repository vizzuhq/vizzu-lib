try {
	chart.off('background-draw', assets.eventHandler)
} catch (error) {
	if (!error.toString().includes('unknown event handler')) {
		throw error
	}
}
chart = chart.animate({
	config: {
		title: 'Add background image'
	}
})

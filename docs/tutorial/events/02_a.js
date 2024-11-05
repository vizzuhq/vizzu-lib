try {
	chart.off('plot-axis-label-draw', assets.eventHandler)
} catch (error) {
	if (!error.toString().includes('unknown event handler')) {
		throw error
	}
}
chart = chart.animate({
	config: {
		title: 'Changing the canvas context before label draw'
	}
})

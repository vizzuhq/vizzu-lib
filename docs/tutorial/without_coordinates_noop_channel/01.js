chart.animate({
	config: {
		channels: {
			y: {
				set: null
			},
			x: {
				set: null
			},
			size: {
				attach: ['Genres', 'Popularity']
			}
		}
	}
})

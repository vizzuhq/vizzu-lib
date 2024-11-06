chart.animate({
	config: {
		channels: {
			y: {
				detach: ['Kinds']
			},
			x: {
				set: ['Genres', 'Kinds']
			}
		}
	}
})

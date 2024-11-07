chart.animate({
	config: {
		channels: {
			y: {
				detach: ['Popularity']
			},
			x: {
				attach: ['Popularity']
			}
		}
	}
})

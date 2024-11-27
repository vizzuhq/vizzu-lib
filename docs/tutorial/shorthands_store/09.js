chart.animate({
	style: null,
	config: {
		title: 'Store function',
		channels: {
			y: {
				set: ['Popularity', 'Kinds'],
				align: 'stretch',
			},
			x: ['Genres'],
			label: ['Popularity']
		},
		color: ['Kinds']
	}
})

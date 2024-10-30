chart.animate({
	config: {
		title: 'Switch to ascending order...',
		channels: {
			y: { set: ['Popularity', 'Kinds'] },
			x: { set: 'Genres' },
			color: { set: 'Kinds' },
			label: { set: 'Popularity' }
		}
	}
})

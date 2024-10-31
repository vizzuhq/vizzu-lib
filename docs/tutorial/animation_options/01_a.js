chart.animate({
	config: {
		title: 'Default options - step 1',
		channels: {
			y: { set: ['Popularity', 'Kinds'] },
			x: { set: 'Genres' },
			color: { set: 'Kinds' },
			label: { set: 'Popularity' }
		}
	}
})

chart.animate({
	config: {
		title: 'Aggregate',
		channels: {
			y: { set: ['Popularity', 'Kinds'] },
			x: { set: 'Genres' },
			color: { set: 'Kinds' },
			label: { set: 'Popularity' }
		}
	}
})

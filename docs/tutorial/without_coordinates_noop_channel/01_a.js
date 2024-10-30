chart.animate({
	config: {
		title: 'Treemap',
		channels: {
			y: { set: ['Popularity', 'Kinds'] },
			x: { set: 'Genres' },
			color: { set: 'Kinds' },
			label: { set: 'Popularity' }
		}
	}
})

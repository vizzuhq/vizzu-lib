chart.animate({
	config: {
		title: 'Switch the orientation = arrange by other axis',
		channels: {
			y: { set: ['Popularity', 'Kinds'] },
			x: { set: 'Genres' },
			color: { set: 'Kinds' },
			label: { set: 'Popularity' }
		}
	}
})

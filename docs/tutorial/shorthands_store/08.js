chart.animate(
	{
		style: null,
		config: {
			title: 'Store function',
			align: 'stretch',
			channels: {
				y: { set: ['Popularity', 'Kinds'] },
				x: { set: 'Genres' },
				label: { attach: 'Popularity' }
			},
			color: { set: 'Kinds' }
		}
	},
	0
)

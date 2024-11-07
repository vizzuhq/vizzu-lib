chart.animate(
	{
		style: null,
		config: {
			title: 'Store function',
			align: 'stretch',
			channels: {
				y: ['Popularity', 'Kinds'],
				x: 'Genres',
				label: { attach: 'Popularity' }
			},
			color: 'Kinds'
		}
	},
	0
)

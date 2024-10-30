chart.animate(
	{
		config: {
			channels: {
				x: { detach: ['Kinds'], attach: ['Genres'] }
			}
		}
	},
	{ regroupStrategy: 'fade' }
)

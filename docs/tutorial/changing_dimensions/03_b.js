chart.animate(
	{
		config: {
			channels: {
				x: { detach: ['Genres'], attach: ['Kinds'] }
			}
		}
	},
	{ regroupStrategy: 'drilldown' }
)

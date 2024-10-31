chart.animate(
	{
		config: {
			channels: {
				x: {
					detach: 'Kinds'
				},
				y: {
					attach: 'Kinds'
				}
			}
		}
	},
	{
		duration: '500ms'
	}
)

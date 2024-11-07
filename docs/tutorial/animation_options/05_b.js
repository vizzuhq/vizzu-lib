chart.animate(
	{
		config: {
			channels: {
				x: {
					attach: ['Kinds']
				},
				y: {
					detach: ['Kinds']
				}
			}
		}
	},
	{
		duration: 1,
		easing: 'linear',
		y: {
			duration: 2,
			delay: 2
		},
		style: {
			duration: 2,
			delay: 4
		}
	}
)

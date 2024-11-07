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

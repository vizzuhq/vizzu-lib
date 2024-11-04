chart
	.animate(
		{
			config: {
				channels: {
					x: {
						detach: ['Kinds']
					},
					y: {
						attach: ['Kinds']
					}
				}
			}
		},
		{
			playState: 'paused',
			position: 0.5
		}
	)
	.activated.then((control) => control.play())

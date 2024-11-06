chart.animate([
	{
		target: {
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
		options: {
			duration: 0.5
		}
	},
	{
		target: {
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
		options: {
			duration: 1
		}
	}
])

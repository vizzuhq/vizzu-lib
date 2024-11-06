chart
	.animate({
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
	})
	.activated.then((control) => control.seek('50%'))

var animation

chart
	.animate({
		channels: {
			// x: { attach: [ 'Kinds' ] },
			x: {
				attach: 'Kinds'
			},
			// y: { detach: [ 'Kinds' ] },
			y: {
				detach: 'Kinds'
			}
		},
		align: 'none'
	})
	.activated.then((control) => {
		animation = control.store()
	})

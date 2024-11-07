chart.animate(
	{
		config: {
			title: 'Measure only',
			channels: {
				x: {
					set: 'M',
					range: { max: 7 },
					axis: true,
					labels: true,
					title: 'x: [ measure ]',
					interlacing: false,
					markerGuides: true,
					ticks: true
				},
				y: {
					set: null,
					axis: true,
					labels: false,
					title: null,
					interlacing: false
				},
				color: 'D',
				label: ['D']
			}
		}
	},
	{
		duration: 2,
		delay: 3
	}
)

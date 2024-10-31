chart.animate(
	{
		config: {
			title: 'Dimension only',
			channels: {
				x: {
					set: ['D', 'i'],
					range: { max: 3 },
					axis: true,
					labels: false,
					title: 'x: [ dimension ]',
					interlacing: false,
					markerGuides: true,
					ticks: false
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
			},
			legend: null
		}
	},
	{
		duration: 2,
		delay: 3
	}
)

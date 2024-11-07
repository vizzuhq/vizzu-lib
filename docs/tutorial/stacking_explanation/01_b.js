chart.animate(
	{
		config: {
			title: 'Measure and dimension',
			channels: {
				x: {
					set: ['D', 'M'],
					range: { max: 12 },
					axis: true,
					labels: true,
					title: 'x: [ measure, dimension ]',
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

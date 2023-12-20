const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Database', type: 'dimension' },
				{ name: 'New page', type: 'measure' }
			]
		}

		const url = 'https://stream.wikimedia.org/v2/stream/page-create'
		const eventSource = new EventSource(url)

		eventSource.onmessage = (event) => {
			const data = JSON.parse(event.data)
			chart.animate(
				{
					data: {
						records: [[data.database, 1]]
					}
				},
				0.1
			)
		}

		return chart.animate({
			data,
			config: {
				y: 'Database',
				x: 'New page',
				title: 'Realtime Wikimedia Data'
			}
		})
	}
]

export default testSteps

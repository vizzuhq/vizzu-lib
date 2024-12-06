const data = {
	series: [
		{
			name: 'Genres',
			type: 'dimension',
			values: ['Pop', 'Rock', 'Jazz', 'Metal', 'Pop', 'Rock', 'Metal', 'Rock', 'Jazz']
		},
		{
			name: 'Kinds',
			type: 'dimension',
			values: [
				'Hard',
				'Hard',
				'Hard',
				'Hard',
				'Smooth',
				'Smooth',
				'Smooth',
				'Experimental',
				'Experimental'
			]
		}
	]
}

const testSteps = [
	(chart) => {
		chart.feature('tooltip', true)
		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: 'Genres',
			y: { set: ['Kinds', 'Genres'], split: true },
			color: 'Kinds',
			lightness: 'Genres'
		})
]

export default testSteps

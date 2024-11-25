const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [15, 32, 12] },
				{ name: 'Baz', values: [5, 3, 2] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: { set: 'Foo', title: 'Valami', ticks: true, guides: true },
			y: { set: 'Bar', title: 'Valami', ticks: true, guides: true, interlacing: false },
			geometry: 'circle',
			sort: 'byValue'
		})
]

export default testSteps

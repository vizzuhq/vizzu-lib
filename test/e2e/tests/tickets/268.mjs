const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob','Alice', 'Bob'] },
				{ name: 'Foo2', values: ['A', 'A', 'B', 'B'] },
				{ name: 'Bar', values: [15, 32, 12, 23] },
			]
		}

		return chart.animate({ data })
	},
	(chart) => chart.animate(
		{
			x: ['Foo','Foo2'],
			y: 'max(Bar)',
			color: 'Foo2'
	}),
	(chart) => chart.animate(
		{
			x: 'Foo',
			y: 'max(Bar)',
			color: null
		})
]

export default testSteps

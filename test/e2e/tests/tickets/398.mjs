const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted', 'Alice', 'Bob', 'Ted', 'Alice'] },
				{ name: 'Foo2', values: ['A', 'A', 'A', 'B', 'B', 'B', 'A'] },
				{ name: 'Foo3', values: ['X', 'X', 'X', 'X', 'X', 'X', 'Y'] },
				{ name: 'Baz', values: [17, 5, 2, 10, 30, 31, 10] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: { set: ['Foo', 'Foo2'], sort: 'byValue' },
			y: 'Baz',
			color: 'Foo',
			geometry: 'circle'
		}),
	(chart) =>
		chart.animate({
			y: ['Foo3', 'Baz']
		})
]

export default testSteps

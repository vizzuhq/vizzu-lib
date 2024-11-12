const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [15, 32, 28] },
				{ name: 'Baz', values: [5, 3, 2] }
			]
		}

		return chart.animate({ data })
	},
	(chart) => chart.animate(
		{
			x: ['Foo', 'Baz'],
			y: { set: ['Bar'], range: { max: 20 } } ,
			label: 'Bar',
			color: 'Foo',
			coordSystem: 'polar',
		}
	)
]

export default testSteps

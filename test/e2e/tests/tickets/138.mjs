const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['A', 'B', 'C','A','B', 'C'] },
				{ name: 'Foo2', values: ['1', '1', '1','2','2', '2'] },
				{ name: 'Bar', values: [15, 32, 12, 3, 2,4] }
			]
		}

		return chart.animate({ data })
	},
	(chart) => chart.animate(
		{
			x: 'Foo',
			color: 'Foo2',
			y: { set: ['Bar','Foo2'], range: { max: 1.2 }},
			align: 'stretch'
	})
]

export default testSteps

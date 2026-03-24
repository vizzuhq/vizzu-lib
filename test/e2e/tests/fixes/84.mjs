const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['A', 'B', 'C', 'A', 'B', 'C', 'B'] },
				{ name: 'Foo2', values: ['1', '1', '1', '2', '2', '2', '3'] },
				{ name: 'Bar', values: [1, 2, 1, 2, 1, 2, 1.5] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate(
			{
				x: ['Foo', 'Foo2'],
				y: 'Bar',
				coordSystem: 'polar'
			},
			0
		),
	(chart) =>
		chart.animate(
			{
				x: { range: { max: 5 } }
			},
			0.2
		),
	(chart) =>
		chart.animate(
			{
				x: { range: { max: 0.68 } }
			},
			0.2
		),
	(chart) =>
		chart.animate({
			coordSystem: 'cartesian'
		}),
	(chart) =>
		chart.animate({
			x: { range: { max: '100%' } }
		})
]

export default testSteps

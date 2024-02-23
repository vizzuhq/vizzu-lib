const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Alice', 'Ted'] },
				{ name: 'Bar', values: ['x', 'y', 'z'] },
				{ name: 'Baz', values: [5, 3, 2] }
			]
		}
		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: 'Foo',
			y: ['min(Baz)', 'Bar'],
			color: 'Bar'
		})
]

export default testSteps

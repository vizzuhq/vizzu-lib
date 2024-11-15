const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted', 'Alice', 'Bob', 'Ted'] },
				{ name: 'Foo3', values: ['a', 'a', 'b', 'b', 'c', 'c'] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			color: ['Foo']
		}),
	(chart) =>
		chart.animate({
			color: ['Foo3'],
			label: ['Foo']
		})
]

export default testSteps

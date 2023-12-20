/** Title should fade in beside the chart, not after. */

const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['a', 'b', 'c', 'a', 'b', 'c'] },
				{ name: 'Foo2', values: ['A', 'A', 'A', 'B', 'B', 'B'] },
				{ name: 'Bar', values: [15, 32, 12, 3, 2] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			color: 'Foo2',
			x: { set: ['Foo2', 'Foo'], labelLevel: 1 },
			y: 'Bar'
		})
]

export default testSteps

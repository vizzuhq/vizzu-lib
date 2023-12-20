/** Title should not disapear from empty chart. */

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
	(chart) => chart.animate({ title: 'Test' }),
	(chart) =>
		chart.animate({
			x: 'Foo',
			y: 'Bar'
		})
]

export default testSteps

const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: ['Happy', 'Happy', 'Sad'] },
				{ name: 'Baz', values: [1, 2, 3] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: { set: 'Foo' },
			y: { set: 'Bar', title: 'Feeling' },
			size: { set: 'Foo' },
			legend: 'size'
		}),
	(chart) =>
		chart.animate({
			x: { title: 'Who' },
			y: { title: null },
			size: { title: 'Bigness' }
		}),
	(chart) =>
		chart.animate({
			x: { set: 'Baz' },
			size: { set: 'Baz' }
		})
]

export default testSteps

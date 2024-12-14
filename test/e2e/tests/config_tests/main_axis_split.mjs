const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: ['Happy', 'Happy', 'Sad'] },
				{ name: 'Baz', values: [1, 2, 3] },
				{ name: 'Bau', values: [4, 3, 2] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			config: {
				x: { set: 'Foo', split: true },
				y: { set: 'Bar' }
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				x: { set: 'Foo' },
				y: { set: ['Bar', 'Bau'] }
			}
		})
]

export default testSteps

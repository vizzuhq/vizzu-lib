const testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series: [
					{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
					{ name: 'Bar', values: [15, 32, 12] }
				]
			}
		}),
	(chart) =>
		chart.animate({
			y: 'Foo',
			x: 'Bar'
		}),
	(chart) =>
		chart.animate({
			color: 'Foo',
			y: ['Bar', 'Foo'],
			x: null
		})
]

export default testSteps

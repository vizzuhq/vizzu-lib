const testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series: [
					{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
					{ name: 'Bar', values: [15, 32, 12] },
					{ name: 'Baz', values: [5, 3, 2] }
				]
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				x: 'Foo',
				y: 'Bar',
				label: 'Bar'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				x: { range: { min: -2, max: 2 } }
			},
			style: {
				paddingRight: 100
			}
		})
]

export default testSteps

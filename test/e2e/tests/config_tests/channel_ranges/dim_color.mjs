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
			color: { set: 'Bar' }
		}),
	(chart) =>
		chart.animate({
			color: { range: { min: '100%', max: '0%'} }
		}),
	(chart) =>
		chart.animate({
			color: {range: { min: '0', max: '0' } },
			label: 'Foo'
		}),
	(chart) =>
		chart.animate({
			x: { attach: 'Baz' },
			color: { range: { min: '-25%', max: '125%'} },
			y: 'Foo'
		}),
	(chart) =>
		chart.animate({
			color: { set: 'Foo', range: { min: '25%', max: '75%' } },
			legend: 'color'
		})
]

export default testSteps

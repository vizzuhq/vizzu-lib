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
			y: { set: 'Bar' }
		}),
	(chart) =>
		chart.animate({
			x: { range: { min: '100%', max: '0%'} },
			y: { range: { min: '100%', max: '0%'} }
		}),
	(chart) =>
		chart.animate({
			x: { attach: 'Baz',  range: { min: 'auto', max: 'auto' } },
			y: { range: { min: '-100%', max: '110%'} },
			label: 'Foo',
			color: 'Foo'
		}),
	(chart) =>
		chart.animate({
			y: {range: { min: '1', max: '1' } }
		}),
	(chart) =>
		chart.animate({
			y: {range: { min: '100%', max: '-100%' } }
		})
]

export default testSteps

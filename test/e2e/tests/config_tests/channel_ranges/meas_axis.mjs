const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Baz', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [5, 7, 9] },
				{ name: 'Foo', values: [1, 2, 3] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate(
			{
				x: { set: 'Foo', step: 0 },
				y: { set: 'Bar' }
			},
			0
		),
	(chart) =>
		chart.animate({
			x: { range: { min: '100%', max: '0%' } },
			y: { range: { min: '100%', max: '0%' } }
		}),
	(chart) =>
		chart.animate({
			x: { attach: 'Baz', range: { min: 'auto', max: 'auto' } },
			y: { range: { min: '-100%', max: '110%' }, step: 1 },
			label: 'Foo',
			color: 'Foo'
		}),
	(chart) =>
		chart.animate({
			x: { range: { min: '15%', max: '55%' } },
			y: { range: { min: '1', max: '1' }, step: 'auto' }
		}),
	(chart) =>
		chart.animate({
			y: { range: { min: '4', max: '4' } }
		}),
	(chart) =>
		chart.animate({
			y: { range: { min: '50%', max: '-100%' }, step: 2 }
		})
]

export default testSteps

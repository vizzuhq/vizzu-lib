const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted', 'Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: ['A', 'A', 'A', 'B', 'B', 'B'] },
				{ name: 'Baz', values: [1, 2, 3, 6, 5, 4] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: 'Foo',
			y: 'Bar',
			size: 'Baz',
			geometry: 'line',
			coordSystem: 'polar'
		}),
	(chart) =>
		chart.animate({
			orientation: 'vertical'
		}),
	(chart) =>
		chart.animate({
			geometry: 'circle',
			orientation: 'horizontal'
		}),
	(chart) =>
		chart.animate(
			{
				orientation: 'vertical'
			},
			1
		),
	(chart) =>
		chart.animate(
			{
				geometry: 'line',
				orientation: 'horizontal',
				coordSystem: 'cartesian'
			},
			1
		),
	(chart) =>
		chart.animate({
			orientation: 'vertical'
		}),
	(chart) =>
		chart.animate({
			geometry: 'circle',
			orientation: 'horizontal'
		}),
	(chart) =>
		chart.animate({
			orientation: 'vertical'
		})
]

export default testSteps

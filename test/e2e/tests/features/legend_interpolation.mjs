const data = {
	series: [
		{ name: 'Foo', unit: 'A', values: [1, 1, 2, 3, 4, 5] },
		{ name: 'Foo2', unit: 'b', values: [10, 10, 20, 30, 40, 50] },
		{ name: 'Bar', values: ['A', 'B', 'C', 'D', 'E', 'F'] },
		{ name: 'Bar2', values: ['A', 'B', 'C', 'D', 'E', 'F'] }
	]
}

const testSteps = [
	(chart) => chart.animate({ data }),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => record.Bar !== 'D'
			},
			config: {
				color: 'Bar',
				legend: 'color',
				x: 'Bar2',
				y: 'Foo2'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => record.Bar !== 'B' && record.Bar !== 'E'
			},
			config: {
				color: null,
				lightness: 'Bar',
				legend: 'lightness'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				lightness: 'Foo'
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				lightness: 'Foo2'
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) => record.Bar !== 'B' && record.Bar !== 'E' && record.Bar !== 'F'
			},
			config: {
				lightness: null,
				color: 'Foo2',
				legend: 'color'
			}
		})
]

export default testSteps

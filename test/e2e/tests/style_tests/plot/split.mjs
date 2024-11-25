const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Bob', 'Ted'] },
				{ name: 'Bar', values: ['Happy', 'Happy', 'Sad', 'Sad'] },
				{ name: 'Baz', values: [1, 2, 3, 4] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: { set: ['Bar'] },
			y: { set: ['Baz', 'Foo'], labelLevel: 0 },
			split: true
		}),
	(chart) =>
		chart.animate({
			config: {
				y: { labelLevel: 1 }
			},
			style: {
				plot: {
					yAxis: {
						split: '15%'
					}
				}
			}
		})
]

export default testSteps

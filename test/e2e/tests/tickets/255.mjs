const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [15, 32, 12] },
				{ name: 'Baz', values: [5, 3, 2] }
			]
		}

		return chart.animate({ data })
	},
	(chart) => chart.animate(
		{
			config: {
				x: 'Foo',
				y: { set: 'Bar', markerGuides: true },
				geometry: 'area'
			},
			style: {
				plot: {
					marker: {
						fillOpacity: 0.3,
						guides: {
							lineWidth: 2,
							color: '#FF0000'
						}
					}
				}
			}
	})
]

export default testSteps

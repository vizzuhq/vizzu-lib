/** Marker label color should be transformed. */

const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [15, 32, 12] }
			]
		}

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			x: 'Foo',
			y: 'Bar',
			color: 'Foo',
			label: 'Bar'
		}),
	(chart) =>
		chart.animate({
			style: {
				plot: {
					marker: {
						label: {
							color: '#00FF00'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			style: {
				plot: {
					marker: {
						label: {
							color: '#FF000030'
						}
					}
				}
			}
		})
]

export default testSteps

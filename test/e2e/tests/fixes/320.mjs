/** empty array should reset channel even without shorthand plugin */

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
	(chart) => {
		chart.feature('shorthands', false)
		return chart
	},
	(chart) =>
		chart.animate([
			{
				target: {
					config: {
						channels: {
							label: { set: [{ name: 'Foo' }] }
						}
					}
				}
			}
		]),
	(chart) =>
		chart.animate([
			{
				target: {
					config: {
						channels: {
							label: { set: [] }
						}
					}
				}
			}
		])
]

export default testSteps

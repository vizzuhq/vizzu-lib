const testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series: [
					{ name: 'Foo', type: 'dimension' },
					{ name: 'Bar', type: 'dimension' }
				]
			},
			config: {
				size: ['Foo']
			},
			style: {
				plot: {
					marker: {
						rectangleSpacing: 0
					}
				}
			}
		})
]

testSteps.push(
	...[...Array(36).keys()].map(
		(i) => (chart) =>
			chart.animate(
				{
					data: {
						records: [[`Foo${i}`, `${i}`]]
					}
				},
				0.1
			)
	)
)

testSteps.push((chart) => chart.animate({ config: { color: 'Bar', legend: null } }, 0.5))

testSteps.push(
	...[...Array(36).keys()].map(
		(i) => (chart) =>
			chart.animate(
				{
					data: {
						filter: (record) => parseInt(record.Bar) > i
					}
				},
				0.1
			)
	)
)

export default testSteps

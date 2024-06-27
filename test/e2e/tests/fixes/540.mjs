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
				size: ['Foo'],
				label: ['Foo']
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
testSteps.push((chart) =>
	chart.animate(
		{
			config: { geometry: 'circle' },

			style: {
				plot: {
					marker: {
						label: {
							position: 'top'
						}
					}
				}
			}
		},
		0.5
	)
)

testSteps.push(
	...[...Array(6).keys()].map(
		(i) => (chart) =>
			chart.animate(
				{
					data: {
						filter: (record) => Math.floor(parseInt(record.Bar) / 4) % 6 > i
					}
				},
				0.2
			)
	)
)

export default testSteps

const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: Array.from(Array(120).keys()).map((x) => x+ ".") },
				{ name: 'Bar', values: [100, 100, 100, 100, 100, 100, 65].concat(Array.from(Array(113).keys()).map(x => 1)) }
			]
		}

		return chart.animate({ data })
	},
	(chart) => chart.animate(
		{
			size: 'Bar',
			color: 'Foo',
			geometry: 'circle',
			legend: null
		}
	)

]

export default testSteps

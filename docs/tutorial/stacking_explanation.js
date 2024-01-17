const mdChartLoaded = import('../assets/javascripts/mdchart.js')
const animOptions = {
	duration: 2,
	delay: 3
}

Promise.all([mdChartLoaded]).then((results) => {
	const MdChart = results[0].default
	const data = {
		series: [
			{ name: 'D', values: ['A', 'B', 'C'] },
			{ name: 'M', values: [6.1, 3.5, 1.4] },
			{ name: 'i', values: [1, 1, 1] }
		]
	}
	const mdchart = new MdChart(data, 'tutorial')

	mdchart.create([
		// tutorial_01
		{
			anims: [
				(chart) =>
					chart.animate(
						{
							config: {
								title: 'Dimension only',
								channels: {
									x: {
										set: ['D', 'i'],
										range: { max: 3 },
										axis: true,
										labels: false,
										title: 'x: [ dimension ]',
										interlacing: false,
										markerGuides: true,
										ticks: false
									},
									y: {
										set: null,
										axis: true,
										labels: false,
										title: null,
										interlacing: false
									},
									color: 'D',
									label: ['D']
								},
								legend: null
							}
						},
						{ ...animOptions, delay: 0 }
					),
				(chart) =>
					chart.animate(
						{
							config: {
								title: 'Measure and dimension',
								channels: {
									x: {
										set: ['D', 'M'],
										range: { max: 12 },
										axis: true,
										labels: true,
										title: 'x: [ measure, dimension ]',
										interlacing: false,
										markerGuides: true,
										ticks: true
									},
									y: {
										set: null,
										axis: true,
										labels: false,
										title: null,
										interlacing: false
									},
									color: 'D',
									label: ['D']
								}
							}
						},
						animOptions
					),
				(chart) =>
					chart.animate(
						{
							config: {
								title: 'Measure only',
								channels: {
									x: {
										set: 'M',
										range: { max: 7 },
										axis: true,
										labels: true,
										title: 'x: [ measure ]',
										interlacing: false,
										markerGuides: true,
										ticks: true
									},
									y: {
										set: null,
										axis: true,
										labels: false,
										title: null,
										interlacing: false
									},
									color: 'D',
									label: ['D']
								}
							}
						},
						animOptions
					),
				(chart) =>
					chart.animate(
						{
							config: {
								title: 'Dimension only',
								channels: {
									x: {
										set: ['D', 'i'],
										range: { max: 3 },
										axis: true,
										labels: false,
										title: 'x: [ dimension ]',
										interlacing: false,
										markerGuides: true,
										ticks: false
									},
									y: {
										set: null,
										axis: true,
										labels: false,
										title: null,
										interlacing: false
									},
									color: 'D',
									label: ['D']
								},
								legend: null
							}
						},
						animOptions
					)
			]
		}
	])
})

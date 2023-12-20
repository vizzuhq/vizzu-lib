const testSteps = [
	async (chart) => {
		const data = {
			series: [
				{
					name: 'category',
					type: 'dimension',
					values: [
						'5',
						'12',
						'3',
						'14',
						'9',
						'10',
						'0',
						'1',
						'13',
						'7',
						'2',
						'4',
						'6',
						'8',
						'11'
					]
				},
				{
					name: 'value',
					type: 'measure',
					values: [5, 12, 3, 14, 9, 10, 0, 1, 13, 7, 2, 4, 6, 8, 11]
				},
				{
					name: 'index',
					type: 'measure',
					values: [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
				}
			]
		}

		let i = 0
		let j = 0

		function lessAt(i) {
			const index = data.series[2].values
			const i0 = index.indexOf(i)
			const i1 = index.indexOf(i + 1)
			const values = data.series[1].values
			return values[i1] < values[i0]
		}

		function swapAt(i) {
			const index = data.series[2].values
			const i0 = index.indexOf(i)
			const i1 = index.indexOf(i + 1)
			const tmp = index[i0]
			index[i0] = index[i1]
			index[i1] = tmp
			return true
		}

		function bubbleSortStep() {
			const length = data.series[1].values.length
			if (j >= length - i - 1) {
				i++
				j = 0
			}
			if (i >= length) return false
			if (lessAt(j)) return swapAt(j)
			else {
				j++
				return bubbleSortStep()
			}
		}

		function update(chart) {
			const swapped = bubbleSortStep()
			if (swapped) return chart.animate({ data }, '50ms').then((chart) => update(chart))
			else {
				return chart
			}
		}

		return chart
			.animate(
				{
					data,
					config: {
						title: 'How Bubble Sort Algorithm Works',
						x: {
							set: 'index',
							labels: false,
							ticks: false,
							markerGuides: true,
							range: {
								min: -0.5,
								max: 14.5
							}
						},
						y: {
							set: 'value',
							markerGuides: false,
							interlacing: false,
							axis: false,
							labels: false,
							title: null
						},
						color: 'value',
						label: 'category',
						geometry: 'circle',
						legend: null
					},
					style: {
						plot: {
							marker: {
								circleMinRadius: 0.02,
								guides: {
									color: '#cccccc',
									lineWidth: 12
								}
							},
							overflow: 'visible'
						}
					}
				},
				0
			)
			.then((chart) => update(chart))
	}
]

export default testSteps

import Chart from '../../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_1',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '1em'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_2',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '2em'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_3',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '3em'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_1-3',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '1em'
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '3em'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_3-1',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '3em'
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '1em'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

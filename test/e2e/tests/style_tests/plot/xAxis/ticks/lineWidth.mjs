import Chart from '../../../../chart.mjs'

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
								lineWidth: 1
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_10',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								lineWidth: 10
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_100',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								lineWidth: 100
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_1-100',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								lineWidth: 1
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
								lineWidth: 100
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_100-1',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								lineWidth: 100
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
								lineWidth: 1
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

import Chart from '../../../../../chart.mjs'

const testCases = [
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
								length: '10%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_15',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '15%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_20',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '20%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_10-20',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '10%'
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
								length: '20%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_20-10',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								length: '20%'
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
								length: '10%'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

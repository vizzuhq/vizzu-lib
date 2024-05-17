import Chart from '../../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_1',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '1%'
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
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '3%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_6',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '6%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_1-6',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '1%'
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '6%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_6-1',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '6%'
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '1%'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

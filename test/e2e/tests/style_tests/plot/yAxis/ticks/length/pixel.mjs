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
								length: '1px'
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
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '10px'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_50',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '50px'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_1-50',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '1px'
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
								length: '50px'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_50-1',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
							ticks: {
								length: '50px'
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
								length: '1px'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

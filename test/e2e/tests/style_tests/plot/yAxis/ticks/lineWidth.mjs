import Chart from '../../../../chart.mjs'

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
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
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
		testName: 'animated_1-10',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
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
						yAxis: {
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
		testName: 'animated_10-1',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometryVerticalOrientation',
				style: {
					plot: {
						yAxis: {
							ticks: {
								lineWidth: 10
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

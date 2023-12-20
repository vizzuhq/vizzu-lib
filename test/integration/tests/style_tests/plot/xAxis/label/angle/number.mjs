import Chart from '../../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_1.5708',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: 1.5708
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_8.6394',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: 8.6394
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_1.5708-0.7854',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: 1.5708
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							label: {
								angle: 0.7854
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_1.5708-2.3562',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: 1.5708
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							label: {
								angle: 2.3562
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_1.5708-3.927',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: 1.5708
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							label: {
								angle: 3.927
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

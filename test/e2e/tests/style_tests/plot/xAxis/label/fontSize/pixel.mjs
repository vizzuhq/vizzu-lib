import Chart from '../../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_1',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '1px'
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
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '10px'
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
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '50px'
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
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '1px'
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
								fontSize: '50px'
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
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '50px'
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
								fontSize: '1px'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

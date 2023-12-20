import Chart from '../../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_0.25',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '0.25turn'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_1.375',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '1.375turn'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_0.25-0.125',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '0.25turn'
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
								angle: '0.125turn'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_0.25-0.375',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '0.25turn'
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
								angle: '0.375turn'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_0.25-0.625',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '0.25turn'
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
								angle: '0.625turn'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

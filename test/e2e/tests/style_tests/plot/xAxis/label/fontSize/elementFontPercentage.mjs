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
								fontSize: '1em'
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
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '2em'
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
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '3em'
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
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '1em'
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
								fontSize: '3em'
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
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '3em'
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
								fontSize: '1em'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

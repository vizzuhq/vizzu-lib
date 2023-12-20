import Chart from '../../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_100',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '100%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_200',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '200%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_300',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '300%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_100-300',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '100%'
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
								fontSize: '300%'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_300-100',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontSize: '300%'
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
								fontSize: '100%'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

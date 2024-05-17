import Chart from '../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_15',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								paddingTop: 15
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_35',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								paddingTop: 35
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_15-35',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								paddingTop: 15
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
								paddingTop: 35
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_35-15',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								paddingTop: 35
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
								paddingTop: 15
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

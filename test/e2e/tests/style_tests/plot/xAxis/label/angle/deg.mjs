import Chart from '../../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_90',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '90deg'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_495',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '495deg'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_90-45',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '90deg'
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
								angle: '45deg'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_90-135',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '90deg'
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
								angle: '135deg'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_90-225',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '90deg'
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
								angle: '225deg'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

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
								angle: '1.5708rad'
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
								angle: '8.6394rad'
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
								angle: '1.5708rad'
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
								angle: '0.7854rad'
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
								angle: '1.5708rad'
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
								angle: '2.3562rad'
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
								angle: '1.5708rad'
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
								angle: '3.927rad'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

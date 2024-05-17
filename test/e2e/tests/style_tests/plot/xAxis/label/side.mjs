import Chart from '../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_negative',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								side: 'negative'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_positive',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								side: 'positive'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_negative-positive',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								side: 'negative'
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
								side: 'positive'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_positive-negative',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								side: 'positive'
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
								side: 'negative'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

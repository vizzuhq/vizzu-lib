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
								paddingBottom: 15,
								position: 'max-edge',
								side: 'positive'
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
								paddingBottom: 35,
								position: 'max-edge',
								side: 'positive'
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
								paddingBottom: 15,
								position: 'max-edge',
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
								paddingBottom: 35
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
								paddingBottom: 35,
								position: 'max-edge',
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
								paddingBottom: 15
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

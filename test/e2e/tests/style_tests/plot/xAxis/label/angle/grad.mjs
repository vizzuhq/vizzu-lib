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
								angle: '100grad'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_550',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '550grad'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_100-50',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '100grad'
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
								angle: '50grad'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_100-150',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '100grad'
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
								angle: '150grad'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_100-250',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								angle: '100grad'
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
								angle: '250grad'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

import Chart from '../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_horizontal',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								orientation: 'horizontal'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_normal',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								orientation: 'normal'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_tangential',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								orientation: 'tangential'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_vertical',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								orientation: 'vertical'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_horizontal-vertical',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								orientation: 'horizontal'
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
								orientation: 'vertical'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_normal-tangential',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								orientation: 'normal'
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
								orientation: 'tangential'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_tangential-normal',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								orientation: 'tangential'
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
								orientation: 'normal'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_vertical-horizontal',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								orientation: 'vertical'
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
								orientation: 'horizontal'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

import Chart from '../../../../chart.mjs'

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
								fontWeight: 100
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_400',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontWeight: 400
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_700',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontWeight: 700
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_900',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontWeight: 900
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_bold',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontWeight: 'bold'
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
								fontWeight: 'normal'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_400-700',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontWeight: 400
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
								fontWeight: 700
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_700-400',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontWeight: 700
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
								fontWeight: 400
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_bold-normal',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontWeight: 'bold'
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
								fontWeight: 'normal'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_normal-bold',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontWeight: 'normal'
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
								fontWeight: 'bold'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

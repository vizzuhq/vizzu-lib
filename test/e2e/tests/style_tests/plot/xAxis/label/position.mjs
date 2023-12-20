import Chart from '../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_axis',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								position: 'axis'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_max-edge',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								position: 'max-edge'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_min-edge',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								position: 'min-edge'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_axis-max-edge',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								position: 'axis'
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
								position: 'max-edge'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_axis-min-edge',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								position: 'axis'
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
								position: 'min-edge'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_max-edge-axis',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								position: 'max-edge'
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
								position: 'axis'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_max-edge-min-edge',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								position: 'max-edge'
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
								position: 'min-edge'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_min-edge-axis',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								position: 'min-edge'
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
								position: 'axis'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_min-edge-max-edge',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								position: 'min-edge'
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
								position: 'max-edge'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

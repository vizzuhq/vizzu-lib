import Chart from '../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_center',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								textAlign: 'center'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_left',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								textAlign: 'left'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_right',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								textAlign: 'right'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_center-left',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								textAlign: 'center'
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
								textAlign: 'left'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_center-right',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								textAlign: 'center'
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
								textAlign: 'right'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_left-center',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								textAlign: 'left'
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
								textAlign: 'center'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_left-right',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								textAlign: 'left'
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
								textAlign: 'right'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_right-center',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								textAlign: 'right'
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
								textAlign: 'center'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_right-left',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								textAlign: 'right'
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
								textAlign: 'left'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

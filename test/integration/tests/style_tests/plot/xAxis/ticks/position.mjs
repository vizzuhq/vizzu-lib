import Chart from '../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_center',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'center'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_inside',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'inside'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_outside',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'outside'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_center-inside',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'center'
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'inside'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_center-outside',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'center'
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'outside'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_inside-center',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'inside'
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'center'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_inside-outside',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'inside'
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'outside'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_outside-center',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'outside'
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'center'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_outside-inside',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithThreeValue',
				config: 'simpleDataCircleGeometry',
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'outside'
							}
						}
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						xAxis: {
							ticks: {
								position: 'inside'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

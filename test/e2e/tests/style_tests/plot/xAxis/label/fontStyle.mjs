import Chart from '../../../../chart.mjs'

const testCases = [
	{
		testName: 'static_italic',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontStyle: 'italic'
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
								fontStyle: 'normal'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'static_oblique',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontStyle: 'oblique'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_italic-normal',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontStyle: 'italic'
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
								fontStyle: 'normal'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_italic-oblique',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontStyle: 'italic'
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
								fontStyle: 'oblique'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_normal-italic',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontStyle: 'normal'
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
								fontStyle: 'italic'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_normal-oblique',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontStyle: 'normal'
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
								fontStyle: 'oblique'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_oblique-italic',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontStyle: 'oblique'
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
								fontStyle: 'italic'
							}
						}
					}
				}
			})
		]
	},
	{
		testName: 'animated_oblique-normal',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						xAxis: {
							label: {
								fontStyle: 'oblique'
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
								fontStyle: 'normal'
							}
						}
					}
				}
			})
		]
	}
]

export default testCases

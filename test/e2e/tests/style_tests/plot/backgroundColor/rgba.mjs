import Chart from '../../../chart.mjs'

const testCases = [
	{
		testName: 'static_black_0.00',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(0,0,0,0.00)'
					}
				}
			})
		]
	},
	{
		testName: 'static_black_0.25',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(0,0,0,0.25)'
					}
				}
			})
		]
	},
	{
		testName: 'static_black_0.50',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(0,0,0,0.50)'
					}
				}
			})
		]
	},
	{
		testName: 'static_black_0.75',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(0,0,0,0.75)'
					}
				}
			})
		]
	},
	{
		testName: 'static_black_1.00',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(0,0,0,1.00)'
					}
				}
			})
		]
	},
	{
		testName: 'static_blueCustom_0.00',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(26,43,60,0.00)'
					}
				}
			})
		]
	},
	{
		testName: 'static_blueCustom_0.25',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(26,43,60,0.25)'
					}
				}
			})
		]
	},
	{
		testName: 'static_blueCustom_0.50',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(26,43,60,0.50)'
					}
				}
			})
		]
	},
	{
		testName: 'static_blueCustom_0.75',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(26,43,60,0.75)'
					}
				}
			})
		]
	},
	{
		testName: 'static_blueCustomWithSpaces_0.75',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(26, 43, 60, 0.75)'
					}
				}
			})
		]
	},
	{
		testName: 'static_blueCustom_1.00',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(26,43,60,1.00)'
					}
				}
			})
		]
	},
	{
		testName: 'static_white_0.00',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(255,255,255,0.00)'
					}
				}
			})
		]
	},
	{
		testName: 'static_white_0.25',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(255,255,255,0.25)'
					}
				}
			})
		]
	},
	{
		testName: 'static_white_0.50',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(255,255,255,0.50)'
					}
				}
			})
		]
	},
	{
		testName: 'static_white_0.75',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(255,255,255,0.75)'
					}
				}
			})
		]
	},
	{
		testName: 'static_white_1.00',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(255,255,255,1.00)'
					}
				}
			})
		]
	},
	{
		testName: 'static_yellowCustom_0.00',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(229,212,195,0.00)'
					}
				}
			})
		]
	},
	{
		testName: 'static_yellowCustom_0.25',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(229,212,195,0.25)'
					}
				}
			})
		]
	},
	{
		testName: 'static_yellowCustom_0.50',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(229,212,195,0.50)'
					}
				}
			})
		]
	},
	{
		testName: 'static_yellowCustom_0.75',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(229,212,195,0.75)'
					}
				}
			})
		]
	},
	{
		testName: 'static_yellowCustomWithSpaces_0.75',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(229,212,195,0.75)'
					}
				}
			})
		]
	},
	{
		testName: 'static_yellowCustom_1.00',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(229,212,195,1.00)'
					}
				}
			})
		]
	},
	{
		testName: 'animated_yellowCustom_0.25-blueCustom_0.75',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(229,212,195,0.25)'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						backgroundColor: 'rgba(26,43,60,0.75)'
					}
				}
			})
		]
	},
	{
		testName: 'animated_yellowCustom_0.75-blueCustom_0.25',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgba(229,212,195,0.75)'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						backgroundColor: 'rgba(26,43,60,0.25)'
					}
				}
			})
		]
	}
]

export default testCases

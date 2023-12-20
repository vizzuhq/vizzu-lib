import Chart from '../../../chart.mjs'

const testCases = [
	{
		testName: 'static_black',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(0,0,0)'
					}
				}
			})
		]
	},
	{
		testName: 'static_blue',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(0,0,255)'
					}
				}
			})
		]
	},
	{
		testName: 'static_blueCustom',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(26,43,60)'
					}
				}
			})
		]
	},
	{
		testName: 'static_blueCustomWithSpaces',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(26, 43, 60)'
					}
				}
			})
		]
	},
	{
		testName: 'static_cyan',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(0,255,255)'
					}
				}
			})
		]
	},
	{
		testName: 'static_lime',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(0,255,0)'
					}
				}
			})
		]
	},
	{
		testName: 'static_magenta',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(255,0,255)'
					}
				}
			})
		]
	},
	{
		testName: 'static_red',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(255,0,0)'
					}
				}
			})
		]
	},
	{
		testName: 'static_white',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(255,255,255)'
					}
				}
			})
		]
	},
	{
		testName: 'static_yellow',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(255,255,0)'
					}
				}
			})
		]
	},
	{
		testName: 'static_yellowCustom',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(229,212,195)'
					}
				}
			})
		]
	},
	{
		testName: 'static_yellowCustomWithSpaces',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(229, 212, 195)'
					}
				}
			})
		]
	},
	{
		testName: 'animated_black-white',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(0,0,0)'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						backgroundColor: 'rgb(255,255,255)'
					}
				}
			})
		]
	},
	{
		testName: 'animated_blueCustom-yellowCustom',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(26,43,60)'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						backgroundColor: 'rgb(229,212,195)'
					}
				}
			})
		]
	},
	{
		testName: 'animated_white-black',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(255,255,255)'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						backgroundColor: 'rgb(0,0,0)'
					}
				}
			})
		]
	},
	{
		testName: 'animated_yellowCustom-blueCustom',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'rgb(229,212,195)'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						backgroundColor: 'rgb(26,43,60)'
					}
				}
			})
		]
	}
]

export default testCases

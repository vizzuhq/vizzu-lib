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
						backgroundColor: '#00000000'
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
						backgroundColor: '#00000040'
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
						backgroundColor: '#00000080'
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
						backgroundColor: '#000000BF'
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
						backgroundColor: '#000000FF'
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
						backgroundColor: '#1A2B3C00'
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
						backgroundColor: '#1A2B3C40'
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
						backgroundColor: '#1A2B3C80'
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
						backgroundColor: '#1A2B3CBF'
					}
				}
			})
		]
	},
	{
		testName: 'static_blueCustomLowerCase_0.75',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: '#1a2b3cbf'
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
						backgroundColor: '#1A2B3CFF'
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
						backgroundColor: '#FFFFFF00'
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
						backgroundColor: '#FFFFFF40'
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
						backgroundColor: '#FFFFFF80'
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
						backgroundColor: '#FFFFFFBF'
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
						backgroundColor: '#FFFFFFFF'
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
						backgroundColor: '#E5D4C300'
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
						backgroundColor: '#E5D4C340'
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
						backgroundColor: '#E5D4C380'
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
						backgroundColor: '#E5D4C3BF'
					}
				}
			})
		]
	},
	{
		testName: 'static_yellowCustomLowerCase_0.75',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: '#e5d4c3bf'
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
						backgroundColor: '#E5D4C3FF'
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
						backgroundColor: '#E5D4C340'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						backgroundColor: '#1A2B3CBF'
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
						backgroundColor: '#E5D4C3BF'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						backgroundColor: '#1A2B3C40'
					}
				}
			})
		]
	}
]

export default testCases

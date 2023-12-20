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
						backgroundColor: '#000'
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
						backgroundColor: '#00F'
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
						backgroundColor: '#0FF'
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
						backgroundColor: '#0F0'
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
						backgroundColor: '#F0F'
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
						backgroundColor: '#F00'
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
						backgroundColor: '#FFF'
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
						backgroundColor: '#FF0'
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
						backgroundColor: '#000'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						backgroundColor: '#FFF'
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
						backgroundColor: '#FFF'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						backgroundColor: '#000'
					}
				}
			})
		]
	}
]

export default testCases

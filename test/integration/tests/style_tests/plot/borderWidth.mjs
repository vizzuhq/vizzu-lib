import Chart from '../../chart.mjs'

const testCases = [
	{
		testName: 'static_1',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						borderWidth: 1,
						borderColor: '#000000'
					}
				}
			})
		]
	},
	{
		testName: 'static_10',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						borderWidth: 10,
						borderColor: '#000000'
					}
				}
			})
		]
	},
	{
		testName: 'static_100',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						borderWidth: 100,
						borderColor: '#000000'
					}
				}
			})
		]
	},
	{
		testName: 'animated_1-100',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						borderWidth: 1,
						borderColor: '#000000'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						borderWidth: 100,
						borderColor: '#000000'
					}
				}
			})
		]
	},
	{
		testName: 'animated_100-1',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						borderWidth: 100,
						borderColor: '#000000'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						borderWidth: 1,
						borderColor: '#000000'
					}
				}
			})
		]
	}
]

export default testCases

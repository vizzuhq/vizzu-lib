import Chart from '../../chart.mjs'

const testCases = [
	{
		testName: 'static_150',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						paddingRight: 150
					}
				}
			})
		]
	},
	{
		testName: 'static_350',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						paddingRight: 350
					}
				}
			})
		]
	},
	{
		testName: 'animated_150-350',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						paddingRight: 150
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						paddingRight: 350
					}
				}
			})
		]
	},
	{
		testName: 'animated_350-150',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						paddingRight: 350
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						paddingRight: 150
					}
				}
			})
		]
	}
]

export default testCases

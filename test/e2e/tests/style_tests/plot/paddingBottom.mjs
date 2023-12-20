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
						paddingBottom: 150
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
						paddingBottom: 350
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
						paddingBottom: 150
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						paddingBottom: 350
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
						paddingBottom: 350
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						paddingBottom: 150
					}
				}
			})
		]
	}
]

export default testCases

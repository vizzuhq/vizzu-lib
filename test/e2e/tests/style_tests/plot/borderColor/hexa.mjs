import Chart from '../../../chart.mjs'

const testCases = [
	{
		testName: 'static_blueCustom_0.25',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						borderWidth: 5,
						borderColor: '#1A2B3C40'
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
						borderWidth: 5,
						borderColor: '#1A2B3CBF'
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
						borderWidth: 5,
						borderColor: '#E5D4C340'
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
						borderWidth: 5,
						borderColor: '#E5D4C3BF'
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
						borderWidth: 5,
						borderColor: '#E5D4C340'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						borderColor: '#1A2B3CBF'
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
						borderWidth: 5,
						borderColor: '#E5D4C3BF'
					}
				}
			}),
			Chart.animate({
				style: {
					plot: {
						borderColor: '#1A2B3C40'
					}
				}
			})
		]
	}
]

export default testCases

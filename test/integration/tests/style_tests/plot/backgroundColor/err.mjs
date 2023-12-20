import Chart from '../../../chart.mjs'

const testCases = [
	{
		testName: 'err_invalid_hex_digit-char',
		errorMsg: 'error: invalid hexadecimal digit',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: '#ABCDEG'
					}
				}
			})
		]
	},
	{
		testName: 'err_invalid_hex_digit-spec',
		errorMsg: 'error: invalid hexadecimal digit',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: '#00000!'
					}
				}
			})
		]
	},
	{
		testName: 'err_invalid_hex_digit-empty',
		errorMsg: 'error: invalid color string',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: '#'
					}
				}
			})
		]
	},
	{
		testName: 'err_invalid_hex_digit-shorter',
		errorMsg: 'error: invalid color string',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: '#00000'
					}
				}
			})
		]
	},
	{
		testName: 'err_invalid_hex_digit-longer',
		errorMsg: 'error: invalid color string',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: '#00000'
					}
				}
			})
		]
	},
	{
		testName: 'err_string_color',
		errorMsg: 'error: invalid color string',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: 'simpleData',
				style: {
					plot: {
						backgroundColor: 'red'
					}
				}
			})
		]
	}
]

export default testCases

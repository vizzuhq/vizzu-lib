import Chart from '../chart.mjs'

const testCases = [
	{
		testName: 'static_default',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar'
				}
			})
		]
	},
	{
		testName: 'static_rectangle',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'rectangle'
				}
			})
		]
	},
	{
		testName: 'static_area',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'area'
				}
			})
		]
	},
	{
		testName: 'static_circle',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'circle'
				}
			})
		]
	},
	{
		testName: 'static_line',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'line'
				}
			})
		]
	},
	{
		testName: 'animated_area-circle',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'area'
				}
			}),
			Chart.animate({
				config: {
					geometry: 'circle'
				}
			})
		]
	},
	{
		testName: 'animated_area-line',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'area'
				}
			}),
			Chart.animate({
				config: {
					geometry: 'line'
				}
			})
		]
	},
	{
		testName: 'animated_area-rectangle',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'area'
				}
			}),
			Chart.animate({
				config: {
					geometry: 'rectangle'
				}
			})
		]
	},
	{
		testName: 'animated_line-area',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'line'
				}
			}),
			Chart.animate({
				config: {
					geometry: 'area'
				}
			})
		]
	},
	{
		testName: 'animated_line-circle',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'line'
				}
			}),
			Chart.animate({
				config: {
					geometry: 'circle'
				}
			})
		]
	},
	{
		testName: 'animated_line-rectangle',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'line'
				}
			}),
			Chart.animate({
				config: {
					geometry: 'rectangle'
				}
			})
		]
	},
	{
		testName: 'animated_rectangle-area',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'rectangle'
				}
			}),
			Chart.animate({
				config: {
					geometry: 'area'
				}
			})
		]
	},
	{
		testName: 'animated_rectangle-circle',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'rectangle'
				}
			}),
			Chart.animate({
				config: {
					geometry: 'circle'
				}
			})
		]
	},
	{
		testName: 'animated_rectangle-line',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithTwoValue',
				config: {
					x: 'Foo',
					y: 'Bar',
					geometry: 'rectangle'
				}
			}),
			Chart.animate({
				config: {
					geometry: 'line'
				}
			})
		]
	}
]

export default testCases

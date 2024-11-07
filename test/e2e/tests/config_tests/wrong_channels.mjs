import Chart from '../chart.mjs'

const testCases = [
	{
		testName: 'err_multiple_dim_channel',
		errorMsg: 'error: Multiple dimension at channel x: Foo',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: ['Foo', 'Foo'],
					y: 'Bar'
				}
			})
		]
	},
	{
		testName: 'err_multiple_dim_channel',
		errorMsg: 'error: Multiple dimension at channel x: Foo',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: ['Foo', 'Bar', 'Foo', 'Bar']
				}
			})
		]
	},
	{
		testName: 'err_multiple_meas_channel',
		errorMsg: 'error: Multiple measure at channel x: sum(Bar) and sum(Bar)',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: ['Bar', 'Bar']
				}
			})
		]
	},
	{
		testName: 'err_multiple_meas_dim_aggr_1',
		errorMsg: 'error: Multiple measure at channel x: sum(Bar) and distinct(Foo)',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [
						'Bar',
						{
							aggregator: 'distinct',
							name: 'Foo'
						}
					]
				}
			})
		]
	},
	{
		testName: 'err_multiple_meas_dim_aggr_2',
		errorMsg: 'error: Multiple measure at channel x: sum(Bar) and distinct(Foo)',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [
						'Bar',
						{
							name: 'Foo',
							aggregator: 'distinct'
						}
					]
				}
			})
		]
	},
	{
		testName: 'err_multiple_meas_dim_aggr_3',
		errorMsg: 'error: Multiple measure at channel x: distinct(Foo) and sum(Bar)',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [
						{
							name: 'Foo',
							aggregator: 'distinct'
						},
						'Bar'
					]
				}
			})
		]
	},
	{
		testName: 'err_multiple_meas_dim_aggr_4',
		errorMsg: 'error: Multiple measure at channel x: distinct(Foo) and sum(Bar)',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [
						{
							aggregator: 'distinct',
							name: 'Foo'
						},
						'Bar'
					]
				}
			})
		]
	},
	{
		testName: 'err_multiple_meas_dim_aggr_5',
		errorMsg: 'error: Multiple measure at channel x: count(Bar) and count(Foo)',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [
						{
							aggregator: 'count',
							name: 'Bar'
						},
						{
							name: 'Foo',
							aggregator: 'count'
						}
					]
				}
			})
		]
	},
	{
		testName: 'err_multiple_meas_dim_aggr_6',
		errorMsg: 'error: Multiple measure at channel x: count(Foo) and count(Bar)',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [
						{
							aggregator: 'count',
							name: 'Foo'
						},
						{
							aggregator: 'count',
							name: 'Bar'
						}
					]
				}
			})
		]
	},
	{
		testName: 'err_no_name_1',
		errorMsg: 'error: Aggregator has no set name at channel x: count',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [
						{
							aggregator: 'count'
						},
						'Bar'
					]
				}
			})
		]
	},
	{
		testName: 'err_no_name_2',
		errorMsg: 'error: Aggregator has no set name at channel x: count',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [
						{
							aggregator: 'count'
						}
					],
					y: 'Foo'
				}
			})
		]
	},
	{
		testName: 'err_invalid_member_at_channel',
		errorMsg: 'error: channels.x.set.0.invalid: invalid channel parameter',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [
						{
							aggregator: 'count',
							name: 'Bar',
							invalid: 'invalid'
						}
					],
					y: 'Foo'
				}
			})
		]
	},
	{
		testName: 'err_empty_series',
		errorMsg: 'error: Empty series at channel: x',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [{}]
				}
			})
		]
	},
	{
		testName: 'err_wrong_aggregator',
		errorMsg:
			"error: not an enum name: 'nonexistent', valid name: sum,min,max,mean,count,distinct,exists",
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: [
						{
							name: 'Bar',
							aggregator: 'nonexistent'
						}
					]
				}
			})
		]
	},
	{
		testName: 'err_wrong_detach_dim',
		errorMsg: 'error: Missing detachable dimension at channel x: Foo',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: { detach: 'Foo' }
				}
			})
		]
	},
	{
		testName: 'err_wrong_detach_meas_1',
		errorMsg: 'error: Missing detachable measure at channel x: distinct(Foo)',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: { detach: { name: 'Foo', aggregator: 'distinct' } }
				}
			})
		]
	},
	{
		testName: 'err_wrong_detach_meas_2',
		errorMsg: 'error: Missing detachable measure at channel x: sum(Bar)',
		testSteps: [
			Chart.animate({
				data: 'simpleDataWithOneValue',
				config: {
					x: { attach: { name: 'Bar', aggregator: 'count' }, detach: { name: 'Bar' } }
				}
			})
		]
	}
]

export default testCases

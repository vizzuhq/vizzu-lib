const testSteps = [
	(chart) => {
		const data = {
			series: [
				{
					name: 'Foo',
					values: [
						'A',
						'B',
						'C',
						'A',
						'B',
						'C',
						'A',
						'B',
						'C',
						'A',
						'B',
						'C',
						'A',
						'B',
						'C',
						'B',
						'B',
						'B',
						'B'
					]
				},
				{
					name: 'Foo2',
					values: [
						'1',
						'1',
						'1',
						'2',
						'2',
						'2',
						'3',
						'3',
						'3',
						'4',
						'4',
						'4',
						'5',
						'5',
						'5',
						'1',
						'2',
						'3',
						'4'
					]
				},
				{
					name: 'Foo3',
					values: [
						'0',
						'1',
						'0',
						'2',
						'3',
						'2',
						'0',
						'1',
						'0',
						'2',
						'3',
						'2',
						'0',
						'1',
						'0',
						'4',
						'4',
						'4',
						'4'
					]
				},
				{
					name: 'Bar',
					values: [NaN, 1, NaN, 1, 2, 1, NaN, 1, NaN, 1, 2, 1, NaN, 1, NaN, 0, 0, 0, 0]
				},
				{ name: 'Bar2', values: [2, 2, 2, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2, 2, 4, 2, 4, 2] }
			]
		}

		chart.feature('tooltip', true)
		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			config: {
				x: ['Foo2'],
				y: {
					set: ['Bar2', 'Foo3'],
					align: 'stretch'
				},
				color: ['Foo3'],
				label: ['Bar2']
			}
		}),
	(chart) =>
		chart.animate({
			style: {
				'plot.marker.label.unit': 'percent'
			}
		})
]

export default testSteps

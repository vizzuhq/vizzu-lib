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

		return chart.animate({ data })
	},
	(chart) =>
		chart.animate({
			config: {
				x: {
					set: ['Foo2', 'Bar2'],
					interlacing: true,
					labels: false,
					labelLevel: 1,
					range: { max: '114.3%' }
				},
				y: {
					set: 'Foo',
					interlacing: true,
					labels: false,
					markerGuides: false,
					axis: false
				},
				color: 'Foo3',
				size: 'Bar',
				geometry: 'circle',
				legend: null
			},
			style: {
				backgroundColor: '#4171cd',
				'plot.xAxis.interlacing': { color: '#9e67ab' },
				'plot.yAxis.interlacing': { color: '#f25456' }
			}
		}),
	(chart) =>
		chart.animate({
			data: {
				filter: (record) =>
					record.Foo2 !== '4' &&
					record.Foo2 !== '5' &&
					record.Foo2 !== '1' &&
					record.Foo3 !== '4' &&
					record.Foo !== 'C'
			},
			config: {
				x: { guides: true, range: { max: '133%' } },
				y: { guides: true, range: { max: '150%' }, reverse: true }
			}
		})
]

export default testSteps

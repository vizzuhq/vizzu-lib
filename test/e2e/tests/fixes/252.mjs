function time(y, m, d) {
	return y + (m - 1) / 12 + (d - 1) / 31 / 12
}

const testSteps = [
	(chart) =>
		chart.animate({
			data: {
				series: [
					{
						name: 'Name',
						type: 'dimension',
						values: [
							'Apple Pie',
							'Banana Bread',
							'Cupcake',
							'Donut',
							'Eclair',
							'Froyo',
							'Gingerbread',
							'Honeycomb',
							'Ice Cream Sandwich',
							'Jelly Bean',
							'KitKat',
							'Lollipop',
							'Marshmallow',
							'Nougat',
							'Orea'
						]
					},
					{
						name: 'Time',
						type: 'measure',
						values: [
							time(2008, 9, 23),
							time(2009, 4, 9),
							time(2009, 4, 27),
							time(2009, 9, 15),
							time(2009, 10, 26),
							time(2010, 4, 29),
							time(2010, 12, 6),
							time(2011, 2, 22),
							time(2011, 10, 12),
							time(2012, 4, 12),
							time(2013, 10, 31),
							time(2014, 11, 3),
							time(2015, 10, 5),
							time(2016, 8, 22),
							time(2017, 8, 21)
						]
					},
					{
						name: 'Placement',
						type: 'measure',
						values: [1, -1, 1.5, -1.5, 1, -2, 1.5, -1.5, 1, -1, 1, -1, 1, -1, 1]
					}
				]
			}
		}),
	(chart) =>
		chart.animate({
			config: {
				x: {
					set: 'Time',
					range: { min: 2008.5, max: 2018.1 },
					step: 1,
					ticks: true,
					title: null
				},
				y: {
					set: 'Placement',
					interlacing: false,
					labels: false,
					markerGuides: false,
					title: null
				},
				label: 'Name',
				geometry: 'circle'
			},
			style: {
				plot: {
					xAxis: {
						label: {
							numberFormat: 'none',
							position: 'axis'
						}
					},
					marker: {
						guides: {
							color: '#666666'
						}
					}
				}
			}
		}),
	(chart) =>
		chart.animate({
			x: { step: 2 }
		})
]

export default testSteps

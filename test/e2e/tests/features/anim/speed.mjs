const data = {
	series: [
		{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
		{ name: 'Bar', values: [15, 32, 12] },
		{ name: 'Baz', values: [500, 300, 200] }
	]
}

const testSteps = [
	(chart) =>
		chart.animate(
			{
				data,
				config: {
					x: 'Foo',
					y: 'Bar'
				}
			},
			{ speed: 5 }
		),
	async (chart) => {
		const anim = chart.animate({
			data,
			config: {
				color: 'Foo',
				x: 'Bar',
				y: 'Baz',
				geometry: 'circle'
			}
		})
		const ctrl = await anim.activated
		ctrl.speed = 3
		return anim
	}
]

export default testSteps

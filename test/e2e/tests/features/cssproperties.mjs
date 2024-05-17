const testSteps = [
	(chart) => {
		const data = {
			series: [
				{ name: 'Foo', values: ['Alice', 'Bob', 'Ted'] },
				{ name: 'Bar', values: [15, 32, 12] },
				{ name: 'Baz', values: [5, 3, 2] }
			]
		}

		const style = document.createElement('style')
		style.innerHTML = `
      body {
        --vizzu-plot-yAxis-label-color: #ff0000;
      }
    `
		document.head.appendChild(style)

		chart.feature('cssProperties', true)

		//    const body = document.querySelector('body')
		//    body.style.setProperty('--vizzu-plot-yAxis-label-color', '#ff0000')

		return chart.animate({ data })
	},
	(chart) => {
		return chart.animate({
			x: 'Foo',
			y: 'Bar'
		})
	},
	(chart) => {
		return chart.animate({
			config: { title: 'override' },
			style: { plot: { yAxis: { label: { color: '#00ff00' } } } }
		})
	},
	(chart) => {
		return chart.animate({
			config: { title: 'back' },
			style: null
		})
	}
]

export default testSteps

const data = {
	series: [
		{ name: 'Dim', type: 'dimension', values: ['a', 'b'] },
		{ name: 'Meas', type: 'measure', values: [1, 2] }
	]
}

const testSteps = [
	async (chart) => {
		const container = chart.feature.htmlCanvas.element
		const container2 = document.createElement('canvas')

		container2.style = `
			width: ${container.width}px; 
			height: ${container.height}px; 
			position: absolute; 
			top: ${container.offsetTop}px; 
			left: ${container.offsetLeft}px;
		`
		container.parentElement.appendChild(container2)

		let chart2 = await new chart.constructor(container2).initializing

		await chart2.animate({
			data,
			config: {
				x: 'Dim',
				y: 'Meas'
			}
		})

		let registry

		const res = new Promise((resolve, reject) => {
			registry = new FinalizationRegistry((heldValue) => {
				if (heldValue === 'Chart 2') {
					console.log('Chart 2 is finalized')
					resolve(chart)
				}
			})
			registry.register(chart2, 'Chart 2')
			chart2.detach()
			chart2 = undefined
		})

		if (typeof window.gc === 'function') {
			// garbage collector should be exposed for this via --expose-gc flag
			// which seems to be not working through webdriver
			window.gc()
		} else {
			let counter = 0
			;(function allocateMemory() {
				Array.from({ length: 1000000 }, () => () => {})
				if (counter > 5000) return
				counter++
				setTimeout(allocateMemory)
			})()
		}

		return res
	}
]

export default testSteps

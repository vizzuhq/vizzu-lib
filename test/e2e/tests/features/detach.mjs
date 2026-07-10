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

		// detach() called before initialization has finished must wait for the
		// initialization and complete without error (rather than failing with a
		// not-initialized error).
		const earlyContainer = document.createElement('canvas')
		container.parentElement.appendChild(earlyContainer)
		const earlyChart = new chart.constructor(earlyContainer)
		await earlyChart.detach()
		earlyContainer.remove()

		let chart2 = await new chart.constructor(container2).initializing

		// A user registered event handler must be unregistered by detach()
		// (detach removes it from the wasm chart before the chart is freed;
		// a regression here would throw during the teardown below).
		chart2.on('click', () => {})

		await chart2.animate({
			data,
			config: {
				x: 'Dim',
				y: 'Meas'
			}
		})

		// Schedule an animation but do not wait for it, then detach right away.
		// detach() must settle the animation queue - by cancelling the pending
		// animation or awaiting its completion - and tear the chart down without
		// throwing. The animation promise is intentionally not retained: if the
		// animation completes it resolves with the chart instance, which would
		// otherwise keep chart2 alive and break the finalization check below.
		chart2.animate({ config: { x: 'Meas', y: 'Dim' } }).catch(() => {})
		await chart2.detach()

		// detach() is idempotent: a repeated call resolves without error.
		await chart2.detach()

		// After detach() every public API call must throw VizzuFinalized;
		// the promise returning animate() must reject with it.
		let finalizedError = false
		try {
			chart2.store()
		} catch (err) {
			finalizedError = err.name === 'VizzuFinalized'
		}
		if (!finalizedError) {
			throw new Error('detached chart should throw VizzuFinalized on API calls')
		}

		let animateRejected = false
		await chart2.animate({ config: { x: 'Dim' } }).catch((err) => {
			animateRejected = err.name === 'VizzuFinalized'
		})
		if (!animateRejected) {
			throw new Error('animate() on a detached chart should reject with VizzuFinalized')
		}

		// detach() releases the wasm chart object and drops every reference to
		// the chart, so the JS instance becomes garbage collectable.
		let registry
		let finalized = false

		const res = new Promise((resolve) => {
			registry = new FinalizationRegistry((heldValue) => {
				if (heldValue === 'Chart 2') {
					console.log('Chart 2 is finalized')
					finalized = true
					resolve(chart)
				}
			})
			registry.register(chart2, 'Chart 2')
			chart2 = undefined
		})

		// Keep triggering garbage collection until the finalizer has run.
		// The registry must be referenced from this pending callback chain,
		// otherwise it may be garbage collected itself, in which case its
		// cleanup callback is never called and the test times out.
		;(function triggerGC() {
			if (finalized || !registry) return
			if (typeof window.gc === 'function') {
				// garbage collector should be exposed for this via --expose-gc flag
				// which seems to be not working through webdriver
				window.gc()
			} else {
				// put pressure on the GC by allocating lots of short-lived objects
				Array.from({ length: 1000000 }, () => () => {})
			}
			setTimeout(triggerGC, 20)
		})()

		return res
	}
]

export default testSteps

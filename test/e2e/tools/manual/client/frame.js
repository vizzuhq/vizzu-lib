class TestRunner {
	constructor() {
		this.canvas = document.getElementById('vizzuCanvas')
		this.canvasCtx = this.canvas.getContext('2d', { willReadFrequently: true })
		this.slider = document.getElementById('myRange')
		this.chart = null
		this.testSteps = []
		this.snapshotId = undefined

		this.urlParamsReady = this.setupUrlParams()
		this.chartReady = this.setupChart()
		this.testStepsReady = this.setupTestSteps()
	}

	setupUrlParams() {
		return import('./url.js').then((urlModule) => {
			const Url = urlModule.default
			const url = new Url()
			this.vizzuUrl = TestRunner.getWholeVizzuUrl(url.getQueryParam('vizzuUrl'))
			this.testFile = url.getQueryParam('testFile')
			this.testType = url.getQueryParam('testType')
			this.testIndex = url.getQueryParam('testIndex')
		})
	}

	setupChart() {
		return this.urlParamsReady
			.then(() => import(this.vizzuUrl))
			.then((vizzuModule) => {
				const Vizzu = vizzuModule.default
				this.chart = new Vizzu(this.canvas)
				return this.chart.initializing
			})
			.then((chart) => {
				console.log(chart.version())
				chart.feature('logging', true)
				this.initSlider()
				return import(this.testFile + '.mjs')
			})
			.then((testModule) => {
				const testSteps =
					this.testType === 'single'
						? testModule.default
						: testModule.default[this.testIndex].testSteps
				this.testSteps = testSteps
				return this.chart.initializing
			})
	}

	setupTestSteps() {
		return this.chartReady
			.then(() => {
				setTimeout(() => {
					this.chart.animation.pause()
				}, 0)
				let finished = this.chart.initializing
				for (const step of this.testSteps) {
					finished = finished.then(step)
				}
				return finished
			})
			.catch(console.log)
	}

	static getWholeVizzuUrl(vizzuUrl) {
		if (!vizzuUrl.endsWith('/vizzu.js') && !vizzuUrl.endsWith('/vizzu.min.js')) {
			vizzuUrl = vizzuUrl + '/vizzu.js'
		}
		return vizzuUrl
	}

	initSlider() {
		this.slider.addEventListener('input', (e) => {
			this.setSlider(e.target.value)
		})
		this.chart.on('update', (ev) => {
			this.slider.value = ev.detail.progress * 1000
		})
	}

	setSlider(value) {
		const t = value
		this.chart.animation.pause()
		this.chart.animation.seek(t / 10 + '%')
		this.snapshotId = value
		setTimeout(() => {
			this.snapshot(value)
		}, 15)
	}

	snapshot(value) {
		if (this.snapshotId !== value) return
		document.vizzuImgIndex = 2 * value
		document.vizzuImgData = this.canvasCtx.getImageData(
			0,
			0,
			this.canvas.width,
			this.canvas.height
		)
		document.vizzuImgIndex = 2 * value + 1
	}

	run(chartToRun) {
		if (!chartToRun) {
			chartToRun = this.chart
		}
		chartToRun.animation.play()
	}
}

// eslint-disable-next-line no-var,no-unused-vars
var testRunner = new TestRunner()

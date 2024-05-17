import TestLib from './test-lib.js'
import TestCase from './test-case.js'
import Url from './url.js'
import ImgDiff from '../../../modules/img/imgdiff.js'

class ManualClient {
	constructor() {
		this.url = new Url()

		this.vizzuUrl = document.querySelector('#vizzuUrl')
		this.vizzuRef = document.querySelector('#vizzuRef')
		this.testCase = document.querySelector('#testCase')
		this.frame = document.querySelector('#frame')
		this.frameRef = document.querySelector('#frame-ref')
		this.difCanvas = document.querySelector('#canvas-dif')
		this.replay = document.querySelector('#replay')
		this.play = document.querySelector('#play')
		this.vscode = document.querySelector('#vscode')
		this.validate = document.querySelector('#validate')

		this.setupUserInterface()
		Promise.all([this.populateLibs(), this.populateCases()]).then(() => {
			this.update()
		})
	}

	setupUserInterface() {
		this.setupFilters()
		this.setupSelects()
		this.setupButtons()
	}

	populateLibs() {
		return fetch('/getLibs')
			.then((response) => response.json())
			.then((data) => {
				const testLibs = Object.entries(data).map(([name, url]) => new TestLib(url, name))
				testLibs.forEach((testLib) => {
					testLib.createOption(this.vizzuUrl)
					testLib.createOption(this.vizzuRef)
				})

				const lastSelectedUrl =
					data[this.url.getQueryParam('vizzuUrl')] || localStorage.getItem('vizzuUrl')
				this.vizzuUrl.value = lastSelectedUrl
				if (!this.vizzuUrl.value) this.vizzuUrl.value = data.localhost

				const lastSelectedRefUrl =
					data[this.url.getQueryParam('vizzuRefUrl')] || localStorage.getItem('vizzuRef')
				this.vizzuRef.value = lastSelectedRefUrl
				if (!this.vizzuRef.value) this.vizzuRef.value = data.HEAD || data.localhost
			})
	}

	populateCases() {
		return fetch('/getTests')
			.then((response) => response.json())
			.then((data) => {
				let lastSelected = localStorage.getItem('testCase')
				const testCases = data.map(
					(testData) =>
						new TestCase(
							testData.testConfig,
							testData.testFile,
							testData.testIndex,
							testData.testName,
							testData.testResult,
							testData.testRepo,
							testData.testSuite,
							testData.testType
						)
				)

				testCases.forEach((testCase) => {
					if (
						testCase.testFile === this.url.getQueryParam('testFile') &&
						// eslint-disable-next-line eqeqeq
						testCase.testIndex == this.url.getQueryParam('testIndex')
					) {
						lastSelected = JSON.stringify(testCase)
					}
					testCase.createOption(this.testCase)
				})

				this.testCase.value = lastSelected
				if (!this.testCase.value) this.testCase.value = JSON.stringify(testCases[0])
			})
	}

	update() {
		localStorage.setItem('vizzuUrl', this.vizzuUrl.value)
		localStorage.setItem('vizzuRef', this.vizzuRef.value)
		localStorage.setItem('testCase', this.testCase.value)
		TestCase.populateStyle(this.testCase)
		const testCaseObject = JSON.parse(this.testCase.value)
		this.detachVizzu(this.frame)
		this.frame.src = `frame.html?testFile=${testCaseObject.testFile}&testType=${testCaseObject.testType}&testIndex=${testCaseObject.testIndex}&vizzuUrl=${this.vizzuUrl.value}`
		if (this.vizzuUrl.value !== this.vizzuRef.value) {
			this.difCanvas.style.display = 'inline'
			this.frameRef.style.display = 'inline'
			this.detachVizzu(this.frameRef)
			this.frameRef.src = `frame.html?testFile=${testCaseObject.testFile}&testType=${testCaseObject.testType}&testIndex=${testCaseObject.testIndex}&vizzuUrl=${this.vizzuRef.value}`
			const imgDiff = new ImgDiff(this.frame, this.frameRef, this.difCanvas)
			imgDiff.getDiff()
		} else {
			this.difCanvas.style.display = 'none'
			this.frameRef.style.display = 'none'
		}

		this.connectSliders().then((charts) => {
			setTimeout(() => {
				this.run(charts)
			}, 0)
		})
	}

	setupFilters() {
		this.filterByName = this.setupFilter('#filterByName')
		this.filterByResult = this.setupFilter('#filterByResult')
	}

	setupSelects() {
		this.vizzuUrl.addEventListener('change', () => this.update())
		this.vizzuRef.addEventListener('change', () => this.update())
		this.testCase.addEventListener('change', () => this.update())
	}

	setupButtons() {
		this.replay.addEventListener('click', () => this.update())
		this.play.addEventListener('click', () => this.run([undefined, undefined]))
		this.vscode.addEventListener('click', () => this.openSelectedInVSCode())
		this.validate.addEventListener('click', () => TestCase.validate(this.testCase))
	}

	detachVizzu(iframe) {
		if (typeof iframe?.contentWindow?.testRunner?.chart?.detach === 'function') {
			iframe.contentWindow.testRunner.chart.detach()
		}
	}

	connectSliders() {
		const waitForLoad = new Promise((resolve) => {
			this.frame.addEventListener('load', () => resolve())
		})

		const waitForLoadRef = new Promise((resolve) => {
			if (this.frameRef.style.display !== 'none') {
				this.frameRef.addEventListener('load', () => resolve())
			} else {
				resolve()
			}
		})

		return Promise.all([waitForLoad, waitForLoadRef])
			.then(() =>
				Promise.all([
					this.frame.contentWindow.testRunner.chartReady,
					this.frameRef.contentWindow.testRunner?.chartReady
				])
			)
			.then((setups) => {
				const slider = this.frame.contentWindow.document.getElementById('myRange')
				const sliderRef = this.frameRef.contentWindow.document.getElementById('myRange')
				slider.addEventListener('input', (e) => {
					this.frameRef.contentWindow.testRunner?.setSlider(e.target.value)
				})

				if (this.frameRef.style.display !== 'none') {
					sliderRef.addEventListener('input', (e) => {
						this.frame.contentWindow.testRunner?.setSlider(e.target.value)
					})
				}
				return setups
			})
	}

	run(charts) {
		this.frame.contentWindow.testRunner.run(charts[0])
		if (this.frameRef.style.display !== 'none') {
			this.frameRef.contentWindow.testRunner.run(charts[1])
		}
	}

	setupFilter(id) {
		const filter = document.querySelector(id)
		filter.addEventListener('input', () => {
			this.filterCases()
		})
		return filter
	}

	openSelectedInVSCode() {
		const testCaseObject = JSON.parse(this.testCase.value)
		const testCasePath = testCaseObject.testRepo + testCaseObject.testFile + '.mjs'
		const encodedTestCasePath = encodeURIComponent(testCasePath)
		const vscodeUri = `vscode://file/${encodedTestCasePath}`
		window.open(vscodeUri, '_blank')
	}

	filterCases() {
		const testCaseOptions = this.testCase.options
		for (let i = 0; i < testCaseOptions.length; i++) {
			const testCaseData = JSON.parse(testCaseOptions[i].value)

			const testName = testCaseData.testName.toLowerCase()
			const filterName = this.filterByName.value.toLowerCase()

			const testResult = testCaseData.testResult?.toLowerCase() ?? '-'
			const filterResult = this.filterByResult.value.toLowerCase()

			if (testName.includes(filterName) && testResult.includes(filterResult)) {
				testCaseOptions[i].style.display = 'block'
			} else {
				testCaseOptions[i].style.display = 'none'
			}
		}
	}
}

// eslint-disable-next-line no-unused-vars
const manualClient = new ManualClient()

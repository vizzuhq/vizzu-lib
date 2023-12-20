import { VideoRecorder } from './videorecorder.js'

function catchError(err) {
	console.error(err)
	let errMsg = err.toString()
	if (err.stack !== undefined) {
		errMsg = err.stack
	}
	window.result = { result: 'ERROR', description: errMsg }
	document.title = 'Finished'
}

try {
	const queryString = window.location.search
	const urlParams = new URLSearchParams(queryString)
	const testSuitePath = urlParams.get('testSuitePath')
	const testCasesPath = urlParams.get('testCasesPath')
	const testCase = urlParams.get('testCase')
	const testName = urlParams.get('testName')
	const testType = urlParams.get('testType')
	const testIndex = urlParams.get('testIndex')
	const vizzuUrl = urlParams.get('vizzuUrl')

	import(vizzuUrl)
		.then((vizzuModule) => {
			const Vizzu = vizzuModule.default
			return import(testSuitePath + '/' + testCasesPath + '/' + testCase + '.mjs').then(
				(testCasesModule) => {
					const chart = new Vizzu('vizzuCanvas')
					const videoRecorder = new VideoRecorder('vizzuCanvas', (data) => {
						const a = document.createElement('a')
						a.setAttribute('href', data)
						a.setAttribute(
							'download',
							testCasesPath.replaceAll('/', '___') + '___' + testName + '.webm'
						)
						a.click()
						window.result = { result: 'OK' }
						document.title = 'Finished'
					})
					let testSteps = []
					if (testType === 'single') {
						testSteps = testCasesModule.default
					} else if (testType === 'multi') {
						testSteps = testCasesModule.default[testIndex].testSteps
					}
					const steps = []
					const snapshots = []
					steps.push((chart) => {
						videoRecorder.start()
						return testSteps[0](chart)
					})
					steps.push((chart) => {
						chart.feature.rendering.update()
						return chart
					})
					for (let i = 1; i < testSteps.length; i++) {
						steps.push((chart) => {
							snapshots.push(chart.store())
							return testSteps[i](chart)
						})
					}
					steps.push((chart) => {
						chart.on('update', (event) => {
							window.progress = event.detail.progress
						})
						chart.on('background-draw', (event) => {
							let progress = 0
							if (window.progress) {
								progress = window.progress
							}
							event.renderingContext.fillStyle = '#ffffff'
							const canvas = document.getElementById('vizzuCanvas')
							event.renderingContext.fillRect(
								-1,
								-1,
								canvas.width + 1,
								canvas.height + 1
							)
							event.renderingContext.globalAlpha = 1 - progress
						})
						chart.on('logo-draw', (event) => {
							event.renderingContext.globalAlpha = 1
						})
						return chart
					})
					steps.push((chart) => {
						return chart.animate(
							{ style: { logo: { filter: 'color(#ffffff00)' } } },
							{ duration: '500ms' }
						)
					})

					let promise = chart.initializing
					for (const step of steps) {
						promise = promise.then((chart) => step(chart))
					}
					return promise.then((chart) => {
						setInterval(() => {
							chart.feature.rendering.update()
						}, 50)
						videoRecorder.stop()
					})
				}
			)
		})
		.catch((err) => {
			catchError(err)
		})
} catch (err) {
	catchError(err)
}

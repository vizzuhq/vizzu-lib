function catchError(err, chart) {
	detach(chart)
	console.error(err)
	let errMsg = err.toString()
	if (err.stack !== undefined) {
		errMsg = err.stack
	}
	window.testData = { result: 'ERROR', description: errMsg }
	document.title = 'Finished'
}

function digestMessage(message) {
	return crypto.subtle.digest('SHA-256', message).then((hashBuffer) => {
		const hashArray = Array.from(new Uint8Array(hashBuffer))
		const hashHex = hashArray.map((b) => b.toString(16).padStart(2, '0')).join('')
		return hashHex
	})
}

function getAnimStep(testCasesModule, testType, testIndex) {
	let animStep
	if (testType === 'multi') {
		animStep = testCasesModule[testIndex].animStep
	}
	if (!animStep) {
		animStep = 20
	}
	return animStep
}

function getTestSteps(testCasesModule, testType, testIndex) {
	let testSteps = []
	if (testType === 'single') {
		testSteps = testCasesModule
	} else if (testType === 'multi') {
		testSteps = testCasesModule[testIndex].testSteps
	}
	return testSteps
}

function detach(chart) {
	try {
		if (chart !== undefined) {
			chart.detach()
		}
	} catch (err) {
		console.error(err)
	}
}

window.addEventListener('error', (event) => {
	catchError(event.error)
})

let chart
try {
	const queryString = window.location.search
	const urlParams = new URLSearchParams(queryString)
	const testFile = urlParams.get('testFile')
	const testType = urlParams.get('testType')
	const testIndex = urlParams.get('testIndex')
	const vizzuUrl = urlParams.get('vizzuUrl')
	const refHash = urlParams.get('refHash')
	const createImages = urlParams.get('createImages')
	const testData = { result: '', hash: '', seeks: [], images: [], hashes: [] }

	import(vizzuUrl)
		.then((vizzuModule) => {
			const Vizzu = vizzuModule.default
			return import(testFile + '.mjs').then((testCasesModule) => {
				const animStep = getAnimStep(testCasesModule.default, testType, testIndex)
				const seeks = []
				for (let seek = parseFloat(animStep); seek <= 100; seek += parseFloat(animStep)) {
					seeks.push(seek)
				}
				chart = new Vizzu('vizzuCanvas')
				const initializing = chart.initializing
				return initializing.then((chart) => {
					let promise = Promise.resolve(chart)
					const promises = []
					const testSteps = getTestSteps(testCasesModule.default, testType, testIndex)
					for (let i = 0; i < testSteps.length; i++) {
						console.log(i)
						promise = promise.then((chart) => {
							testData.seeks[i] = []
							testData.images[i] = []
							testData.hashes[i] = []
							const animFinished = testSteps[i](chart)
							if (animFinished === undefined) {
								throw new Error('test step return value is undefined')
							}
							if (animFinished.activated)
								animFinished.activated.then((control) => {
									control.pause()
									seeks.forEach((seek) => {
										seek = seek + '%'
										testData.seeks[i].push(seek)
										control.seek(seek)
										chart.feature.rendering.update()
										const canvasElement = document.getElementById('vizzuCanvas')
										if (createImages !== 'DISABLED') {
											const dataURL = canvasElement.toDataURL()
											testData.images[i].push(dataURL)
										}
										const ctx = canvasElement.getContext('2d')
										const digestData = ctx.getImageData(
											0,
											0,
											canvasElement.width,
											canvasElement.height
										)
										let digest = digestMessage(digestData.data.buffer.slice())
										digest = digest.then((digestBuffer) => {
											testData.hashes[i].push(digestBuffer)
										})
										promises.push(digest)
									})
									control.play()
								})
							return animFinished
						})
					}
					return promise.then(() => {
						chart.detach()
						return Promise.all(promises).then(() => {
							testData.hashes.forEach((items) => {
								testData.hash += items.join('')
							})
							const buf = new ArrayBuffer(testData.hash.length * 2)
							const bufView = new Uint16Array(buf)
							for (let i = 0, strLen = testData.hash.length; i < strLen; i++) {
								bufView[i] = testData.hash.charCodeAt(i)
							}
							digestMessage(bufView).then((hash) => {
								hash = hash.substring(0, 7)
								testData.hash = hash
								if (refHash !== '') {
									if (refHash.includes(hash)) {
										testData.result = 'PASSED'
										if (createImages === 'FAILED') {
											delete testData.images
										}
									} else {
										testData.description =
											'hash: ' +
											testData.hash +
											' ' +
											'(ref: ' +
											refHash +
											')'
										testData.result = 'FAILED'
									}
								} else {
									testData.description =
										'ref hash does not exist (hash: ' + testData.hash + ')'
									testData.result = 'WARNING'
									testData.warning = 'noref'
								}
								if (typeof window.testData === 'undefined') {
									window.testData = testData
									document.title = 'Finished'
								}
							})
						})
					})
				})
			})
		})
		.catch((err) => {
			catchError(err, chart)
		})
} catch (err) {
	catchError(err, chart)
}

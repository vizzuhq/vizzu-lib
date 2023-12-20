const pngjs = require('pngjs')

const path = require('path')
const fs = require('fs')

const TestEnv = require('../../../modules/e2e-test/test-env.cjs')

class TestCaseResult {
	#cnsl

	#testCaseObj
	#testData

	#browserChrome
	#vizzuUrl
	#vizzuRefUrl

	#runTestCaseRef

	#testCaseFormattedName
	#testCaseResultPath

	#imgDiffModuleReady

	constructor(testCaseObj, testData, browserChrome, vizzuUrl, vizzuRefUrl, runTestCaseRef) {
		this.#cnsl = testCaseObj.cnsl

		this.#testCaseObj = testCaseObj
		this.#testData = testData
		this.#browserChrome = browserChrome
		this.#vizzuUrl = vizzuUrl
		this.#vizzuRefUrl = vizzuRefUrl

		this.#runTestCaseRef = runTestCaseRef

		this.#testCaseFormattedName = this.#getTestCaseFormattedName()
		this.#testCaseResultPath = this.#getTestCaseResultPath()

		this.#imgDiffModuleReady = import('../../../modules/img/imgdiff.js')
	}

	#getTestCaseFormattedName() {
		return path.relative(
			TestEnv.getTestSuitePath(),
			path.join(TestEnv.getWorkspacePath(), this.#testCaseObj.testCase.testName)
		)
	}

	#getTestCaseResultPath() {
		return path.join(TestEnv.getTestSuiteResultsPath(), this.#testCaseFormattedName)
	}

	createTestCaseResult() {
		return new Promise((resolve, reject) => {
			const deleteTestCaseResultReady = this.#deleteTestCaseResult()
			deleteTestCaseResultReady.then(() => {
				if (this.#testCaseObj.createImages === 'ALL') {
					this.#createImages()
				}
				if (this.#testCaseObj.testCase.errorMsg) {
					if (this.#testData.result === 'ERROR') {
						if (
							this.#testData.description.includes(this.#testCaseObj.testCase.errorMsg)
						) {
							return resolve(
								this.#createTestCaseResultPassed(
									this.#testCaseObj.testCase.errorMsg
								)
							)
						} else {
							return resolve(this.#createTestCaseResultError())
						}
					} else {
						this.#testData.result = 'ERROR'
						this.#testData.description =
							'did not occur ' + this.#testCaseObj.testCase.errorMsg
						return resolve(this.#createTestCaseResultError())
					}
				} else {
					if (this.#testData.result === 'PASSED') {
						return resolve(this.#createTestCaseResultPassed(this.#testData.hash))
					} else if (
						this.#testData.result === 'WARNING' ||
						this.#testData.result === 'FAILED'
					) {
						return resolve(this.#createTestCaseResultFailure())
					} else {
						return resolve(this.#createTestCaseResultError())
					}
				}
			})
		})
	}

	#deleteTestCaseResult() {
		return new Promise((resolve, reject) => {
			fs.rm(this.#testCaseResultPath, { recursive: true, force: true }, (err) => {
				if (err) {
					return reject(err)
				}
				return resolve()
			})
		})
	}

	#createTestCaseResultPassed(msg) {
		this.#testCaseObj.testSuiteResults.PASSED.push(this.#testCaseObj.testCase.testName)
		this.#cnsl.writePassedLog(' ' + this.#testCaseFormattedName)
		this.#cnsl.log(
			('[ ' + 'PASSED'.padEnd(this.#cnsl.getTestStatusPad(), ' ') + ' ] ').success +
				'[ ' +
				String(++this.#testCaseObj.testSuiteResults.FINISHED).padEnd(
					this.#cnsl.getTestNumberPad(),
					' '
				) +
				' ] ' +
				'[ ' +
				msg +
				' ] ' +
				this.#testCaseFormattedName
		)
	}

	#createTestCaseResultFailure() {
		return new Promise((resolve, reject) => {
			if (this.#testData.result === 'WARNING') {
				if (
					this.#testData.warning === 'noref' &&
					this.#testCaseObj.Werror.includes('noref')
				) {
					this.#createTestCaseResultFailed()
					return resolve()
				}
				this.#createTestCaseResultWarning()
				return resolve()
			} else {
				if (
					this.#vizzuRefUrl &&
					this.#vizzuUrl !== this.#vizzuRefUrl &&
					!this.#maxFailedImagesReached()
				) {
					const testCaseObj = Object.assign({}, this.#testCaseObj)
					testCaseObj.createImages = 'ALL'
					this.#runTestCaseRef(testCaseObj, this.#browserChrome, this.#vizzuRefUrl)
						.then((testDataRef) => {
							const failureMsgs = []
							if (testDataRef.result !== 'ERROR') {
								let diff = false
								for (let i = 0; i < (this.#testData?.hashes?.length ?? 0); i++) {
									for (
										let j = 0;
										j < (this.#testData?.hashes?.[i]?.length ?? 0);
										j++
									) {
										const hashRef = testDataRef?.hashes?.[i]?.[j]
										if (this.#testData.hashes[i][j] !== hashRef) {
											if (this.#testCaseObj.createImages !== 'DISABLED') {
												this.#createImage(testDataRef, '-2ref', i, j)
												this.#createDifImage(testDataRef, i, j)
											}
											failureMsgs.push(
												''.padEnd(this.#cnsl.getTestStatusPad() + 5, ' ') +
													'[ ' +
													'step: ' +
													i +
													'. - seek: ' +
													this.#testData.seeks[i][j] +
													' - hash: ' +
													this.#testData.hashes[i][j].substring(0, 7) +
													' ' +
													'(ref: ' +
													hashRef?.substring(0, 7) +
													')' +
													' ]'
											)
											diff = true
										}
									}
								}
								if (!diff) {
									failureMsgs.push(
										''.padEnd(this.#cnsl.getTestStatusPad() + 5, ' ') +
											'[ the currently counted hashes are the same, the difference is probably caused by the environment ]'
									)
									this.#testData.warning = 'sameref'
								}
								return failureMsgs
							} else {
								const errParts = this.#getTestCaseResultErrorParts(testDataRef)
								const failureMsgs = [
									''.padEnd(this.#cnsl.getTestStatusPad() + 5, ' ') +
										'[ failed to run reference ]'
								]
								if (errParts) {
									failureMsgs[0] += ` [ ${errParts[0]} ]`
									if (errParts.length > 1) {
										errParts.forEach((item, index) => {
											errParts[index] =
												''.padEnd(this.#cnsl.getTestStatusPad() + 7, ' ') +
												item
										})
										failureMsgs.push(...errParts.slice(1))
									}
								}
								return failureMsgs
							}
						})
						.then((failureMsgs) => {
							if (
								this.#testData.warning === 'sameref' &&
								!this.#testCaseObj.Werror.includes('sameref')
							) {
								this.#createTestCaseResultWarning(failureMsgs)
								return resolve()
							}
							this.#createTestCaseResultFailed(failureMsgs)
							return resolve()
						})
				} else {
					this.#createTestCaseResultFailed()
					return resolve()
				}
			}
		})
	}

	#createTestCaseResultWarning(failureMsgs) {
		if (this.#testCaseObj.createImages === 'FAILED' && !this.#maxFailedImagesReached()) {
			this.#createImages()
		}
		this.#testCaseObj.testSuiteResults.WARNING.push(this.#testCaseObj.testCase.testName)
		this.#cnsl.writeWarningsLog(' ' + this.#testCaseFormattedName)
		this.#createTestCaseResultManual()
		this.#cnsl.log(
			(
				'[ ' +
				'WARNING'.padEnd(this.#cnsl.getTestStatusPad(), ' ') +
				' ] ' +
				'[ ' +
				String(++this.#testCaseObj.testSuiteResults.FINISHED).padEnd(
					this.#cnsl.getTestNumberPad(),
					' '
				) +
				' ] ' +
				'[ ' +
				this.#testData.description +
				' ] '
			).warn + this.#testCaseFormattedName
		)
		if (failureMsgs) {
			failureMsgs.forEach((failureMsg) => {
				this.#cnsl.log(failureMsg)
			})
		}
	}

	#createTestCaseResultFailed(failureMsgs) {
		if (this.#testCaseObj.createImages === 'FAILED' && !this.#maxFailedImagesReached()) {
			this.#createImages()
		}
		this.#testCaseObj.testSuiteResults.FAILED.push(this.#testCaseObj.testCase.testName)
		this.#cnsl.writeFailedLog(' ' + this.#testCaseFormattedName)
		this.#createTestCaseResultManual()
		this.#createTestCaseResultErrorMsg()
		if (failureMsgs) {
			failureMsgs.forEach((failureMsg) => {
				this.#cnsl.log(failureMsg)
			})
		}
	}

	#createTestCaseResultError() {
		this.#testCaseObj.testSuiteResults.FAILED.push(this.#testCaseObj.testCase.testName)
		this.#cnsl.writeFailedLog(' ' + this.#testCaseFormattedName)
		this.#createTestCaseResultManual()
		this.#createTestCaseResultErrorMsg()
	}

	#createTestCaseResultErrorMsg() {
		const errParts = this.#getTestCaseResultErrorParts(this.#testData)
		if (errParts) {
			this.#cnsl.log(
				(
					'[ ' +
					this.#testData.result.padEnd(this.#cnsl.getTestStatusPad(), ' ') +
					' ] ' +
					'[ ' +
					String(++this.#testCaseObj.testSuiteResults.FINISHED).padEnd(
						this.#cnsl.getTestNumberPad(),
						' '
					) +
					' ] ' +
					'[ ' +
					errParts[0] +
					' ] '
				).error + this.#testCaseFormattedName
			)
			if (errParts.length > 1) {
				errParts.slice(1).forEach((item) => {
					this.#cnsl.log(''.padEnd(this.#cnsl.getTestStatusPad() + 7, ' ') + item)
				})
			}
		}
	}

	#getTestCaseResultErrorParts(testData) {
		return testData.description
			.split('http://127.0.0.1:' + String(this.#testCaseObj.workspaceHostServerPort))
			.join(path.resolve(TestEnv.getWorkspacePath()))
			.split('\n')
	}

	#createTestCaseResultManual() {
		this.#testCaseObj.testSuiteResults.MANUAL.push(this.#testCaseObj.testCase)
		const formatted = this.#testCaseFormattedName
		this.#testCaseObj.testSuiteResults.MANUAL_FORMATTED.push(formatted)
		this.#cnsl.writeFailuresLog(' ' + formatted)
	}

	#createImages() {
		for (let i = 0; i < (this.#testData?.seeks?.length ?? 0); i++) {
			for (let j = 0; j < (this.#testData?.seeks[i]?.length ?? 0); j++) {
				this.#createImage(this.#testData, '-1new', i, j)
			}
		}
	}

	#createImage(data, fileAdd, i, j) {
		if (!(data?.seeks?.[i]?.[j] && data?.images?.[i]?.[j])) {
			return
		}
		fs.mkdir(this.#testCaseResultPath, { recursive: true, force: true }, (err) => {
			if (err) {
				throw err
			}
			const seek = data.seeks[i][j].replace('%', '').split('.')
			if ((seek.length ?? 0) === 1) {
				seek.push('0')
			}
			fs.writeFile(
				this.#testCaseResultPath +
					'/' +
					path.basename(this.#testCaseResultPath) +
					'_' +
					i.toString().padStart(3, '0') +
					'_' +
					seek[0].padStart(3, '0') +
					'.' +
					seek[1].padEnd(3, '0') +
					'%' +
					fileAdd +
					'.png',
				data.images[i][j].substring(22),
				'base64',
				(err) => {
					if (err) {
						throw err
					}
				}
			)
		})
	}

	#createDifImage(testDataRef, i, j) {
		if (
			!(
				this.#testData?.seeks?.[i]?.[j] &&
				this.#testData?.images?.[i]?.[j] &&
				testDataRef?.images?.[i]?.[j]
			)
		) {
			return
		}
		const seek = this.#testData.seeks[i][j].replace('%', '').split('.')
		if ((seek.length ?? 0) === 1) {
			seek.push('0')
		}
		const img1 = pngjs.PNG.sync.read(
			Buffer.from(this.#testData.images[i][j].substring(22), 'base64')
		)
		const img2 = pngjs.PNG.sync.read(
			Buffer.from(testDataRef.images[i][j].substring(22), 'base64')
		)
		const { width, height } = img1
		this.#imgDiffModuleReady.then((imgDiffModule) => {
			const ImgDiff = imgDiffModule.default
			const compareResult = ImgDiff.compare('move', img1.data, img2.data, width, height)
			if (!compareResult.match) {
				const imgDiff = new pngjs.PNG({ width, height })
				imgDiff.data = compareResult.diffData
				fs.mkdir(this.#testCaseResultPath, { recursive: true, force: true }, (err) => {
					if (err) {
						throw err
					}
					fs.writeFile(
						this.#testCaseResultPath +
							'/' +
							path.basename(this.#testCaseResultPath) +
							'_' +
							i.toString().padStart(3, '0') +
							'_' +
							seek[0].padStart(3, '0') +
							'.' +
							seek[1].padEnd(3, '0') +
							'%' +
							'-3diff' +
							'.png',
						pngjs.PNG.sync.write(imgDiff),
						(err) => {
							if (err) {
								throw err
							}
						}
					)
				})
			}
		})
	}

	#maxFailedImagesReached() {
		if (!this.#testCaseObj.maxFailedImages) {
			return false
		}
		const failedCases =
			this.#testCaseObj.testSuiteResults.FAILED.length +
			this.#testCaseObj.testSuiteResults.WARNING.length
		return failedCases >= this.#testCaseObj.maxFailedImages
	}
}

module.exports = TestCaseResult

const fs = require('fs')

const pLimitReady = import('p-limit')
const AggregateErrorReady = import('aggregate-error')

const WorkspaceHost = require('../../modules/workspace/workspace-host.cjs')
const BrowsersChrome = require('../../modules/browser/browsers-chrome.cjs')
const VizzuUrl = require('../../modules/vizzu/vizzu-url.cjs')
const TestEnv = require('../../modules/e2e-test/test-env.cjs')
const TestConsole = require('../../modules/e2e-test/test-console.cjs')
const TestCase = require('../../modules/e2e-test/test-case/test-case.cjs')
const TestCasesConfig = require('../../modules/e2e-test/test-case/test-cases-config.cjs')
const TestCases = require('../../modules/e2e-test/test-case/test-cases.cjs')
const TestSuiteResult = require('../../modules/e2e-test/test-suite-result.cjs')

class TestSuite {
	#cnsl
	#pLimit

	#browsersChrome
	#browsersChromeReady

	#vizzuUrl
	#vizzuUrlReady
	#vizzuRefUrl
	#vizzuRefUrlReady

	#workspaceHost
	#workspaceHostReady
	#workspaceHostServerPort

	#Werror
	#createImages
	#maxFailedImages
	#createHashes

	#testCasesConfig
	#testCasesConfigReady
	#testCases
	#testCasesReady

	#testSuiteResults = {
		PASSED: [],
		WARNING: [],
		FAILED: [],
		TIME: { START: Math.round(Date.now() / 1000), END: 0 },
		FINISHED: 0,
		MANUAL: [],
		MANUAL_FORMATTED: [],
		RESULTS: {}
	}

	constructor(
		configPathList,
		filters,
		noLogs,
		browsersNum,
		browserGui,
		vizzuUrl,
		vizzuRefUrl,
		Werror,
		createImages,
		maxFailedImages,
		createHashes
	) {
		this.#cnsl = new TestConsole(!noLogs)
		this.#browsersChrome = new BrowsersChrome(
			browsersNum,
			browserGui,
			this.#cnsl.getTestSuiteLogPath(),
			this.#cnsl.getTestSuiteLogPath()
				? this.#cnsl.getTimeStamp()
				: this.#cnsl.getTestSuiteLogPath()
		)

		this.#vizzuUrl = vizzuUrl
		this.#vizzuRefUrl = vizzuRefUrl

		this.#Werror = Werror
		this.#createImages = createImages
		this.#maxFailedImages = maxFailedImages
		this.#createHashes = createHashes

		this.#testCasesConfigReady = TestCasesConfig.getConfig(configPathList)
		this.#testCasesReady = TestCases.getTestCases(this.#testCasesConfigReady, filters)
	}

	static del() {
		fs.rm(TestEnv.getTestSuiteReportPath(), { recursive: true, force: true }, (err) => {
			if (err) {
				throw err
			}
		})
	}

	test() {
		return new Promise((resolve, reject) => {
			this.#runTestSuite()
				.catch((err) => {
					return reject(err)
				})
				.finally(() => {
					this.#destructTestSuite()
					return resolve()
				})
		})
	}

	cnsl() {
		return this.#cnsl
	}

	#runTestSuite() {
		return new Promise((resolve, reject) => {
			this.#testCasesReady.then((testCases) => {
				this.#testCases = testCases
				if (testCases.filteredTestCases.length > 0) {
					this.#startTestSuite().then(() => {
						this.#cnsl.setTestNumberPad(
							String(testCases.filteredTestCases.length).length
						)
						const limit = this.#pLimit.default(this.#browsersChrome.getBrowsersNum())
						const testCasesReady = testCases.filteredTestCases.map(
							(filteredTestCase) => {
								const testCaseObj = {
									testCase: filteredTestCase,
									testCasesConfig: this.#testCasesConfig,
									testSuiteResults: this.#testSuiteResults,
									workspaceHostServerPort: this.#workspaceHostServerPort,
									browsersChrome: this.#browsersChrome,
									Werror: this.#Werror,
									createImages: this.#createImages,
									maxFailedImages: this.#maxFailedImages,
									animTimeout: this.#browsersChrome.getTimeout(),
									cnsl: this.#cnsl
								}
								return limit(() =>
									TestCase.runTestCase(
										testCaseObj,
										this.#vizzuUrl,
										this.#vizzuRefUrl
									)
								)
							}
						)
						Promise.all(testCasesReady)
							.then(() => {
								const testSuiteResult = new TestSuiteResult(
									this.#testSuiteResults,
									this.#testCases,
									this.#testCasesConfig,
									this.#createHashes,
									this.#cnsl
								)
								testSuiteResult.createTestSuiteResult()
								return resolve()
							})
							.catch((err) => {
								return reject(err)
							})
					})
				}
			})
		})
	}

	#startTestSuite() {
		return new Promise((resolve, reject) => {
			const startTestSuiteReady = []

			this.#cnsl.log(
				'[ ' +
					'LOG TIME'.padEnd(this.#cnsl.getTestStatusPad(), ' ') +
					' ]' +
					' ' +
					'[ ' +
					this.#cnsl.getTimeStamp() +
					' ]'
			)

			startTestSuiteReady.push(pLimitReady)
			pLimitReady.then((pLimit) => {
				this.#pLimit = pLimit
			})

			startTestSuiteReady.push(this.#testCasesConfigReady)
			this.#testCasesConfigReady.then((testCasesConfig) => {
				this.#testCasesConfig = testCasesConfig
				testCasesConfig.suites.forEach((suite) => {
					this.#cnsl.log(
						'[ ' +
							'T.CONFIG'.padEnd(this.#cnsl.getTestStatusPad(), ' ') +
							' ]' +
							' ' +
							'[ ' +
							suite.config +
							' ]'
					)
					this.#cnsl.log(
						'[ ' +
							'T.CASES'.padEnd(this.#cnsl.getTestStatusPad(), ' ') +
							' ]' +
							' ' +
							'[ ' +
							suite.suite +
							' ]'
					)
				})
			})

			startTestSuiteReady.push(this.#testCasesReady)
			this.#testCasesReady.then((testCases) => {
				this.#cnsl.log(
					'[ ' +
						'T.CASES'.padEnd(this.#cnsl.getTestStatusPad(), ' ') +
						' ]' +
						' ' +
						'[ ' +
						testCases.filteredTestCases.length +
						' / ' +
						testCases.testCases.length +
						' ]'
				)
			})

			this.#vizzuUrlReady = VizzuUrl.resolveVizzuUrl(
				this.#vizzuUrl,
				TestEnv.getWorkspacePath(),
				TestEnv.getTestSuitePath()
			)
			startTestSuiteReady.push(this.#vizzuUrlReady)
			this.#vizzuUrlReady.then((url) => {
				this.#vizzuUrl = url
				this.#cnsl.log(
					'[ ' +
						'V. URL'.padEnd(this.#cnsl.getTestStatusPad(), ' ') +
						' ]' +
						' ' +
						'[ ' +
						url +
						' ]'
				)
			})

			this.#vizzuRefUrlReady = new Promise((resolve) => {
				return VizzuUrl.resolveVizzuUrl(
					this.#vizzuRefUrl,
					TestEnv.getWorkspacePath(),
					TestEnv.getTestSuitePath()
				)
					.then((url) => {
						return resolve(url)
					})
					.catch(() => {
						return resolve('')
					})
			})
			startTestSuiteReady.push(this.#vizzuRefUrlReady)
			this.#vizzuRefUrlReady.then((url) => {
				this.#vizzuRefUrl = url
				this.#cnsl.log(
					'[ ' +
						'V.R. URL'.padEnd(this.#cnsl.getTestStatusPad(), ' ') +
						' ]' +
						' ' +
						'[ ' +
						url +
						' ]'
				)
			})

			this.#workspaceHost = new WorkspaceHost(TestEnv.getWorkspacePath())
			this.#workspaceHostReady = this.#workspaceHost.serverPortReady()
			startTestSuiteReady.push(this.#workspaceHostReady)
			this.#workspaceHostReady.then((serverPort) => {
				this.#workspaceHostServerPort = serverPort
				this.#cnsl.log(
					'[ ' +
						'W. HOST'.padEnd(this.#cnsl.getTestStatusPad(), ' ') +
						' ]' +
						' ' +
						'[ ' +
						'http://127.0.0.1:' +
						String(serverPort) +
						' ]'
				)
			})

			this.#browsersChrome.setBrowsersNum(
				this.#testCases.filteredTestCases.length < this.#browsersChrome.getBrowsersNum()
					? this.#testCases.filteredTestCases.length
					: this.#browsersChrome.getBrowsersNum()
			)
			this.#cnsl.log(
				'[ ' +
					'BROWSERS'.padEnd(this.#cnsl.getTestStatusPad(), ' ') +
					' ]' +
					' ' +
					'[ ' +
					this.#browsersChrome.getBrowsersNum() +
					' ]'
			)
			this.#browsersChromeReady = this.#browsersChrome.startBrowsers()
			startTestSuiteReady.push(this.#browsersChromeReady)

			Promise.all(startTestSuiteReady)
				.then(() => {
					return resolve()
				})
				.catch((err) => {
					return reject(err)
				})
		})
	}

	#destructTestSuite() {
		AggregateErrorReady.then((AggregateError) => {
			const errs = []
			try {
				this.#browsersChrome.closeBrowsers()
			} catch (err) {
				errs.push(err)
			}
			try {
				if (this.#workspaceHost) {
					this.#workspaceHost.closeServer()
				}
			} catch (err) {
				errs.push(err)
			}
			if (errs.length > 1) {
				throw new AggregateError(errs)
			} else if (errs.length === 1) {
				throw errs[0]
			}
		})
	}
}

module.exports = TestSuite

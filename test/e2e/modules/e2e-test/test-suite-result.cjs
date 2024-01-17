const path = require('path')
const fs = require('fs')
const prettier = require('prettier')

const TestEnv = require('../../modules/e2e-test/test-env.cjs')

class TestSuiteResult {
	#cnsl

	#testSuiteResults
	#testCases
	#testCasesConfig
	#createHashes

	constructor(testSuiteResults, testCases, testCasesConfig, createHashes, cnsl) {
		this.#cnsl = cnsl

		this.#testSuiteResults = testSuiteResults
		this.#testCases = testCases
		this.#testCasesConfig = testCasesConfig
		this.#createHashes = createHashes
	}

	createTestSuiteResult() {
		this.#createNewConfig()
		if (this.#testSuiteResults.MANUAL.length !== 0) {
			this.#cnsl.log('\n')
			this.#testSuiteResults.MANUAL.forEach((testCase) => {
				this.#cnsl.log(
					''.padEnd(this.#cnsl.getTestStatusPad() + 5, ' ') +
						path.relative(
							TestEnv.getTestSuitePath(),
							path.join(TestEnv.getWorkspacePath(), testCase.testName)
						)
				)
				this.#cnsl.log(
					''.padEnd(this.#cnsl.getTestStatusPad() + 5, ' ') +
						'http://127.0.0.1:8080/test/e2e/tools/manual/client?testFile=' +
						testCase.testFile +
						'&testType=' +
						testCase.testType +
						'&testIndex=' +
						testCase.testIndex +
						'&vizzuUrl=localhost\n'
				)
			})
			this.#cnsl.log('\n')
			this.#cnsl.log(
				''.padEnd(this.#cnsl.getTestStatusPad() + 5, ' ') +
					'npm run test:man --' +
					' ' +
					this.#testSuiteResults.MANUAL_FORMATTED.map((s) => `'${s}'`).join(' ')
			)
		}
		this.#testSuiteResults.TIME.END = Math.round(Date.now() / 1000)
		const duration = this.#testSuiteResults.TIME.END - this.#testSuiteResults.TIME.START
		this.#cnsl.log('\n' + 'duration:'.padEnd(15, ' ') + duration + 's')
		this.#cnsl.log('\n' + 'all tests:'.padEnd(15, ' ') + this.#testCases.testCases.length)
		const sum =
			this.#testSuiteResults.PASSED.length +
			this.#testSuiteResults.WARNING.length +
			this.#testSuiteResults.FAILED.length
		this.#cnsl.log('tests run:'.padEnd(15, ' ') + sum)
		this.#cnsl.log(
			('tests passed:'.padEnd(15, ' ') + this.#testSuiteResults.PASSED.length).success
		)
		if (this.#testSuiteResults.WARNING.length !== 0) {
			this.#cnsl.log(
				('tests warning:'.padEnd(15, ' ') + this.#testSuiteResults.WARNING.length).warn
			)
		} else {
			this.#cnsl.log('tests warning:'.padEnd(15, ' ') + this.#testSuiteResults.WARNING.length)
		}
		if (this.#testSuiteResults.FAILED.length !== 0) {
			this.#cnsl.log(
				('tests failed:'.padEnd(15, ' ') + this.#testSuiteResults.FAILED.length).error
			)
			process.exitCode = 1
		} else {
			this.#cnsl.log('tests failed:'.padEnd(15, ' ') + this.#testSuiteResults.FAILED.length)
		}
	}

	#createNewConfig() {
		if (this.#createHashes !== 'DISABLED') {
			if (
				this.#createHashes === 'ALL' ||
				(this.#createHashes === 'FAILED' &&
					(this.#testSuiteResults.FAILED.length !== 0 ||
						this.#testSuiteResults.WARNING.length !== 0))
			) {
				const testCasesConfig = {}
				this.#testCasesConfig.suites.forEach((suite) => {
					const suitePath = '/' + path.relative(TestEnv.getWorkspacePath(), suite.suite)
					testCasesConfig[suitePath] = {
						suite: suitePath,
						tmp: suite.config,
						test: {}
					}
				})
				for (const [key] of Object.entries(this.#testSuiteResults.RESULTS)) {
					if (
						this.#createHashes === 'FAILED' &&
						!this.#testSuiteResults.FAILED.includes(key) &&
						!this.#testSuiteResults.WARNING.includes(key)
					) {
						continue
					}
					Object.keys(testCasesConfig).forEach((suite) => {
						if (key.startsWith(suite)) {
							if (this.#testSuiteResults.RESULTS[key].hash) {
								testCasesConfig[suite].test[path.relative(suite, key)] = {
									refs: [this.#testSuiteResults.RESULTS[key].hash]
								}
							}
						}
					})
				}

				for (const [key, value] of Object.entries(testCasesConfig)) {
					if (Object.keys(testCasesConfig[key].test).length === 0) {
						continue
					}
					const conFigPath = path.join(
						TestEnv.getTestSuiteResultsPath(),
						path.relative(
							TestEnv.getTestSuitePath(),
							path.join(TestEnv.getWorkspacePath(), key)
						),
						path.basename(value.tmp)
					)
					const rmReady = new Promise((resolve, reject) => {
						fs.rm(conFigPath, { force: true }, (err) => {
							if (err) {
								return reject(err)
							}
							return resolve()
						})
					})
					const mkdirReady = new Promise((resolve, reject) => {
						fs.mkdir(path.dirname(conFigPath), { recursive: true }, (err) => {
							if (err) {
								return reject(err)
							}
							return resolve()
						})
					})
					Promise.all([rmReady, mkdirReady]).then(() => {
						const configData = value
						delete configData.tmp
						configData.test = Object.keys(configData.test)
							.sort()
							.reduce((a, c) => {
								a[c] = configData.test[c]
								return a
							}, {})
						const formattedConfigDataReady = prettier.format(
							JSON.stringify(configData, null, '\t'),
							{
								parser: 'json',
								tabWidth: 4,
								useTabs: true
							}
						)
						formattedConfigDataReady.then((formattedConfigData) => {
							fs.writeFile(conFigPath, formattedConfigData, (err) => {
								if (err) {
									throw err
								}
							})
						})
					})
				}
			}
		}
	}
}

module.exports = TestSuiteResult

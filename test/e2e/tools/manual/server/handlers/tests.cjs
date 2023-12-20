const fs = require('fs').promises

const TestEnv = require('../../../../modules/e2e-test/test-env.cjs')
const TestCasesConfig = require('../../../../modules/e2e-test/test-case/test-cases-config.cjs')
const TestCases = require('../../../../modules/e2e-test/test-case/test-cases.cjs')

class TestsHandler {
	#testCaseModule

	constructor(testCaseModule, res, configPathList, filters) {
		this.res = res
		this.configPathList = configPathList
		this.filters = filters

		this.#testCaseModule = testCaseModule
	}

	handle() {
		const testCasesConfigReady = TestCasesConfig.getConfig(this.configPathList)
		const testResultsReady = this.#getTestResults()
		const testCasesReady = TestCases.getTestCases(testCasesConfigReady, this.filters)

		Promise.all([testCasesReady, testResultsReady])
			.then(([testCases, testResults]) => {
				testCases.filteredTestCases.forEach((testCase) => {
					testCase.testResult = this.#getTestCaseResult(testCase.testName, testResults)
				})
				this.res.send(testCases.filteredTestCases)
			})
			.catch((err) => {
				console.error(err)
				this.res.status(500).send('internal server error')
			})
	}

	#getTestResults() {
		return Promise.all([this.#getPassed(), this.#getWarnings(), this.#getFailed()])
	}

	#getTestCaseResult(testName, testResults) {
		const [passed, warnings, failed] = testResults
		if (passed.includes(testName)) {
			return this.#testCaseModule.TestCaseResult.TYPES.PASSED
		} else if (warnings.includes(testName)) {
			return this.#testCaseModule.TestCaseResult.TYPES.WARNING
		} else if (failed.includes(testName)) {
			return this.#testCaseModule.TestCaseResult.TYPES.FAILED
		}
	}

	#getPassed() {
		const logPath = TestEnv.getTestSuitePassedLog()
		return this.#getLog(logPath)
	}

	#getWarnings() {
		const logPath = TestEnv.getTestSuiteWarningsLog()
		return this.#getLog(logPath)
	}

	#getFailed() {
		const logPath = TestEnv.getTestSuiteFailedLog()
		return this.#getLog(logPath)
	}

	#getLog(logPath) {
		return fs
			.readFile(logPath, 'utf8')
			.then((data) => {
				const prefix = `/${TestEnv.getTestSuiteRelativePath()}/`
				const tests = data
					.trim()
					.split(' ')
					.filter((test) => test !== '')
					.map((test) => prefix + test)
				return tests
			})
			.catch(() => [])
	}
}

module.exports = TestsHandler

const path = require('path')

const TestEnv = require('../../../modules/e2e-test/test-env.cjs')
const TestCaseResult = require('../../../modules/e2e-test/test-case/test-case-result.cjs')

class TestCase {
	static runTestCase(testCaseObj, vizzuUrl, vizzuRefUrl) {
		return new Promise((resolve, reject) => {
			const browserChrome = testCaseObj.browsersChrome.shiftBrowser()
			TestCase.runTestCaseClient(testCaseObj, browserChrome, vizzuUrl).then((testData) => {
				testCaseObj.testSuiteResults.RESULTS[testCaseObj.testCase.testName] = testData
				const testCaseResult = new TestCaseResult(
					testCaseObj,
					testData,
					browserChrome,
					vizzuUrl,
					vizzuRefUrl,
					TestCase.runTestCaseRef
				)
				testCaseResult.createTestCaseResult().then(() => {
					testCaseObj.browsersChrome.pushBrowser(browserChrome)
					return resolve()
				})
			})
		})
	}

	static runTestCaseRef(testCaseObj, browserChrome, vizzuUrl) {
		return new Promise((resolve, reject) => {
			TestCase.runTestCaseClient(testCaseObj, browserChrome, vizzuUrl)
				.then((testDataRef) => {
					return resolve(testDataRef)
				})
				.catch((err) => {
					return reject(err)
				})
		})
	}

	static runTestCaseClient(testCaseObj, browserChrome, vizzuUrl) {
		return new Promise((resolve, reject) => {
			let refHash = []
			if (testCaseObj.testCase.testName in testCaseObj.testCasesConfig.tests) {
				if ('refs' in testCaseObj.testCasesConfig.tests[testCaseObj.testCase.testName]) {
					refHash = testCaseObj.testCasesConfig.tests[testCaseObj.testCase.testName].refs
				}
			}
			if (vizzuUrl.startsWith('/')) {
				vizzuUrl = '/' + path.relative(TestEnv.getWorkspacePath(), vizzuUrl)
			}
			browserChrome
				.getUrl(
					'http://127.0.0.1:' +
						String(testCaseObj.workspaceHostServerPort) +
						'/test/e2e/modules/e2e-test/client/index.html' +
						'?testFile=' +
						testCaseObj.testCase.testFile +
						'&testType=' +
						testCaseObj.testCase.testType +
						'&testIndex=' +
						testCaseObj.testCase.testIndex +
						'&vizzuUrl=' +
						vizzuUrl +
						'&refHash=' +
						refHash.toString() +
						'&createImages=' +
						testCaseObj.createImages
				)
				.then(() => {
					browserChrome
						.waitUntilTitleIs('Finished', testCaseObj.animTimeout)
						.then(() => {
							browserChrome
								.executeScript(() => {
									return testData // eslint-disable-line no-undef
								})
								.then((testData) => {
									return resolve(testData)
								})
						})
						.catch((err) => {
							if (!err.toString().includes('TimeoutError: Waiting failed')) {
								throw err
							}
							return resolve({ result: 'ERROR', description: 'Timeout' })
						})
				})
		})
	}
}

module.exports = TestCase

const path = require('path')
const fs = require('fs')

const assert = require('../../../modules/console/assert.cjs')
const WorkspacePath = require('../../../modules/workspace/workspace-path.cjs')
const TestEnv = require('../../../modules/e2e-test/test-env.cjs')
const TestCasesConfig = require('../../../modules/e2e-test/test-case/test-cases-config.cjs')

class TestCases {
	static getTestCases(testCasesConfigReady, filters) {
		return new Promise((resolve, reject) => {
			testCasesConfigReady.then((configs) => {
				try {
					assert(
						TestCasesConfig.isTestCasesConfig(configs),
						'test cases config schema validation failed'
					)
					const testCasesReadyList = []
					const filteredTestCasesReadyList = []
					configs.suites.forEach((suite) => {
						const testCasesReady = TestCases.collectTestCases(suite, suite.suite)
						testCasesReadyList.push(testCasesReady)
						testCasesReady
							.then((testCases) => {
								const filteredTestCasesReady = TestCases.filterTestCases(
									testCases,
									suite.suite,
									filters
								)
								filteredTestCasesReadyList.push(filteredTestCasesReady)
							})
							.catch((err) => {
								return reject(err)
							})
					})
					Promise.all(testCasesReadyList)
						.then((testCasesList) => {
							testCasesList = testCasesList.flat(1)
							Promise.all(filteredTestCasesReadyList).then(
								(filteredTestCasesList) => {
									filteredTestCasesList = filteredTestCasesList.flat(1)
									testCasesList.sort((a, b) => (a.testName > b.testName ? 1 : -1))
									filteredTestCasesList.sort((a, b) =>
										a.testName > b.testName ? 1 : -1
									)
									return resolve({
										testCases: testCasesList,
										filteredTestCases: filteredTestCasesList
									})
								}
							)
						})
						.catch((err) => {
							return reject(err)
						})
				} catch (err) {
					return reject(err)
				}
			})
		})
	}

	static collectTestCases(suite, p, testCases = []) {
		return new Promise((resolve, reject) => {
			fs.lstat(p, (err, stats) => {
				if (err) {
					return reject(err)
				} else {
					if (stats.isDirectory()) {
						fs.readdir(p, (err, items) => {
							if (err) {
								return reject(err)
							} else {
								const testCasesReady = []
								items.forEach((item) => {
									const testCaseReady = TestCases.collectTestCases(
										suite,
										path.join(p, item),
										testCases
									)
									testCasesReady.push(testCaseReady)
									testCaseReady
										.then((newTestCases) => {
											testCases = testCases.concat(newTestCases)
										})
										.catch((err) => {
											return reject(err)
										})
								})
								Promise.all(testCasesReady)
									.then(() => {
										return resolve(testCases)
									})
									.catch((err) => {
										return reject(err)
									})
							}
						})
					} else {
						if (path.extname(p) === '.mjs') {
							TestCases.preprocessTestCases(suite, p)
								.then((testCases) => {
									return resolve(testCases)
								})
								.catch((err) => {
									return reject(err)
								})
						} else {
							return resolve([])
						}
					}
				}
			})
		})
	}

	static filterTestCases(testCases, suitePath, filters = []) {
		return new Promise((resolve, reject) => {
			let filteredTestCases = []
			if (filters.length === 0) {
				filteredTestCases = testCases
			} else {
				const testKeys = {}
				testCases.forEach((testCase) => {
					if (testCase.testName !== testCase.testFile) {
						if (testKeys[testCase.testFile] !== undefined) {
							testKeys[testCase.testFile].push(testCase)
						} else {
							testKeys[testCase.testFile] = [testCase]
						}
					}
					testKeys[testCase.testName] = [testCase]
				})
				filters.forEach((filter) => {
					const parsedFilter = path.parse(filter)
					if (parsedFilter.ext === '.mjs') {
						filter = path.join(parsedFilter.dir, parsedFilter.name)
					}
					if (path.dirname(filter) === '.') {
						testCases.forEach((testCase) => {
							if (
								path.basename(filter) === path.basename(testCase.testName) ||
								path.basename(filter) === path.basename(testCase.testFile)
							) {
								filteredTestCases.push(testCase)
							}
						})
					} else {
						if (testKeys[filter]) {
							filteredTestCases = filteredTestCases.concat(testKeys[filter])
						} else {
							const filterPathInSuite =
								'/' +
								path.join(
									path.relative(TestEnv.getWorkspacePath(), suitePath),
									filter
								)
							const filterRelative =
								'/' +
								path.relative(
									TestEnv.getWorkspacePath(),
									WorkspacePath.resolvePath(
										filter,
										TestEnv.getWorkspacePath(),
										TestEnv.getTestSuitePath()
									)
								)
							const filterAbsolute =
								'/' + path.relative(TestEnv.getWorkspacePath(), filter)
							if (testKeys[filterPathInSuite]) {
								filteredTestCases = filteredTestCases.concat(
									testKeys[filterPathInSuite]
								)
							} else if (testKeys[filterRelative]) {
								filteredTestCases = filteredTestCases.concat(
									testKeys[filterRelative]
								)
							} else if (testKeys[filterAbsolute]) {
								filteredTestCases = filteredTestCases.concat(
									testKeys[filterAbsolute]
								)
							}
						}
					}
				})
			}
			return resolve(filteredTestCases)
		})
	}

	static preprocessTestCases(suite, p) {
		return new Promise((resolve, reject) => {
			const testCase = path.relative(TestEnv.getWorkspacePath(), p)
			const testCaseWoExt = path.join(
				'/',
				path.dirname(testCase),
				path.basename(testCase, '.mjs')
			)
			TestCases.validateTestCaseName(testCaseWoExt)
			TestCases.importTestCase(p).then((testCaseContent) => {
				if (testCaseContent) {
					testCaseContent = testCaseContent.default
				}
				if (!Array.isArray(testCaseContent))
					// eslint-disable-next-line prefer-promise-reject-errors
					return reject(p + ': test case file validation failed')
				if (testCaseContent.length === 0)
					// eslint-disable-next-line prefer-promise-reject-errors
					return reject(p + ': test case file validation failed')
				let testCasestype
				let testCasestypesOK = true
				testCaseContent.forEach((testCaseContentItem) => {
					if (typeof testCaseContentItem === 'object') {
						if (testCaseContentItem === null || Array.isArray(testCaseContentItem)) {
							testCasestypesOK = false
						}
					} else if (typeof testCaseContentItem !== 'function') {
						testCasestypesOK = false
					}
					if (!testCasestype) {
						testCasestype = typeof testCaseContentItem
					} else {
						// eslint-disable-next-line valid-typeof
						if (testCasestype !== typeof testCaseContentItem) testCasestypesOK = false
					}
				})
				if (!testCasestypesOK) {
					// eslint-disable-next-line prefer-promise-reject-errors
					return reject(p + ': test case file validation failed')
				}
				const workspacePath = TestEnv.getWorkspacePath()
				const suiteRelPath = path.relative(workspacePath, suite.suite)
				if (testCasestype === 'function') {
					return resolve([
						{
							testFile: testCaseWoExt,
							testType: 'single',
							testName: testCaseWoExt,
							testIndex: undefined,
							testRepo: workspacePath,
							testSuite: suiteRelPath,
							testConfig: suite.config
						}
					])
				} else {
					const testCaseContentItems = []
					testCaseContent.forEach((element, index) => {
						testCaseContentItems.push({
							testFile: testCaseWoExt,
							testType: 'multi',
							testName: testCaseWoExt + '/' + element.testName,
							testIndex: index,
							testRepo: workspacePath,
							testSuite: suiteRelPath,
							testConfig: suite.config,
							errorMsg: element.errorMsg
						})
					})
					return resolve(testCaseContentItems)
				}
			})
		})
	}

	static importTestCase(p) {
		return import(p)
	}

	static validateTestCaseName(p) {
		const invalidChars = /[ ()]/

		if (invalidChars.test(p)) {
			throw new Error(`failed to load test case (contains invalid char): ${p}`)
		}
	}
}

module.exports = TestCases

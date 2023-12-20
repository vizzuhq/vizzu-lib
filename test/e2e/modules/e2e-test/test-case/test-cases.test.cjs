const path = require('path')
const fs = require('fs')

const TestEnv = require('../test-env.cjs')
const TestCases = require('./test-cases.cjs')

const suites = path.join(__dirname, '../../../test_report/unit/test-cases')
const testPathList = [
	path.join(suites, 'suite1', 'notest1.js'),
	path.join(suites, 'suite1', 'test1.mjs'),
	path.join(suites, 'suite1', 'testgroup1a', 'test1a.mjs'),
	path.join(suites, 'suite1', 'testgroup1a', 'test1aa.mjs'),
	path.join(suites, 'suite1', 'testgroup1a', 'testgroup1b', 'test1b.mjs'),
	path.join(suites, 'suite1', 'testgroup1a', 'testgroup1b', 'test1bb.mjs'),
	path.join(suites, 'suite1', 'testgroup1a', 'testgroup1b', 'testgroup1c', 'test1c.mjs'),
	path.join(suites, 'suite2', 'test2.mjs'),
	path.join(suites, 'suite2', 'testgroup2a', 'test2a.mjs'),
	path.join(suites, 'suite2', 'testgroup2a', 'test2aa.mjs'),
	path.join(suites, 'suite2', 'testgroup2a', 'testgroup2b', 'test2b.mjs'),
	path.join(suites, 'suite2', 'testgroup2a', 'testgroup2b', 'test2bb.mjs'),
	path.join(suites, 'suite2', 'testgroup2a', 'testgroup2b', 'testgroup2c', 'test2c.mjs'),
	path.join(suites, 'suite2', 'testgroup2d', 'test2d.mjs'),
	path.join(suites, 'suite2', 'testgroup2d', 'notest2.js')
]

const testCaseList = [
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite1',
		testFile: '/test/e2e/test_report/unit/test-cases/suite1/test1',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite1/test1',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite1',
		testFile: '/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/test1a',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/test1a',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite1',
		testFile: '/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/test1aa',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/test1aa',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite1',
		testFile: '/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/test1b',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/test1b',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite1',
		testFile: '/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/test1bb',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/test1bb',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite1',
		testFile:
			'/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/testgroup1c/test1c',
		testIndex: undefined,
		testName:
			'/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/testgroup1c/test1c',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite2',
		testFile: '/test/e2e/test_report/unit/test-cases/suite2/test2',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite2/test2',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite2',
		testFile: '/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/test2a',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/test2a',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite2',
		testFile: '/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/test2aa',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/test2aa',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite2',
		testFile: '/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/test2b',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/test2b',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite2',
		testFile: '/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/test2bb',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/test2bb',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite2',
		errorMsg: undefined,
		testFile:
			'/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c',
		testIndex: 0,
		testName:
			'/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c/c1',
		testType: 'multi'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite2',
		errorMsg: undefined,
		testFile:
			'/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c',
		testIndex: 1,
		testName:
			'/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c/c2',
		testType: 'multi'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite2',
		testFile: '/test/e2e/test_report/unit/test-cases/suite2/testgroup2d/test2d',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite2/testgroup2d/test2d',
		testType: 'single'
	}
]

const filteredTestCaseList = [
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite1',
		testFile: '/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/test1bb',
		testIndex: undefined,
		testName: '/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/test1bb',
		testType: 'single'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite2',
		errorMsg: undefined,
		testFile:
			'/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c',
		testIndex: 0,
		testName:
			'/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c/c1',
		testType: 'multi'
	},
	{
		testConfig: '',
		testRepo: path.join(__dirname, '../../../../..'),
		testSuite: 'test/e2e/test_report/unit/test-cases/suite2',
		errorMsg: undefined,
		testFile:
			'/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c',
		testIndex: 1,
		testName:
			'/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c/c2',
		testType: 'multi'
	}
]

const testCasesConfigReady = new Promise((resolve, reject) => {
	return resolve({
		suites: [
			{
				suite: './test/e2e/test_report/unit/test-cases/suite1',
				config: '',
				tests: {}
			},
			{
				suite: './test/e2e/test_report/unit/test-cases/suite2',
				config: '',
				tests: {}
			}
		],
		tests: {}
	})
})

beforeEach(() => {
	jest.useFakeTimers()
	jest.resetAllMocks()
	jest.spyOn(TestCases, 'importTestCase').mockImplementation((p) => {
		if (
			p ===
			'test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c.mjs'
		) {
			return new Promise((resolve, reject) => {
				return resolve({
					default: [
						{
							testName: 'c1',
							testSteps: [() => {}]
						},
						{
							testName: 'c2',
							testSteps: [() => {}]
						}
					]
				})
			})
		} else {
			return new Promise((resolve, reject) => {
				return resolve({ default: [() => {}] })
			})
		}
	})
})

beforeAll(() => {
	return new Promise((resolve, reject) => {
		const testsReady = []
		testPathList.forEach((testPath) => {
			// eslint-disable-next-line promise/param-names
			const testReady = new Promise((testResolve, testReject) => {
				fs.rm(suites, { recursive: true, force: true }, (err) => {
					if (err) {
						testReject(err)
					}
					fs.mkdir(path.dirname(testPath), { force: true, recursive: true }, (err) => {
						if (err) {
							testReject(err)
						}
						fs.open(testPath, 'w', (err) => {
							if (err) {
								testReject(err)
							}
							testResolve()
						})
					})
				})
			})
			testsReady.push(testReady)
		})

		Promise.all(testsReady)
			.then(() => {
				resolve()
			})
			.catch((err) => {
				reject(err)
			})
	})
})

describe('getTestCases()', () => {
	describe('testCasesConfigReady', () => {
		test('if undefined, err is thrown', () => {
			return expect(TestCases.getTestCases()).rejects.toThrow(
				"Cannot read properties of undefined (reading 'then')"
			)
		})

		test('if not a promise, err is thrown', () => {
			return expect(TestCases.getTestCases('testCasesConfigReady')).rejects.toThrow(
				'testCasesConfigReady.then is not a function'
			)
		})

		test('if does not fit into schema, err is thrown', () => {
			const testCasesConfigReady = new Promise((resolve, reject) => {
				return resolve({})
			})
			return expect(TestCases.getTestCases(testCasesConfigReady)).rejects.toThrow(
				'Assert failed: test cases config schema validation failed'
			)
		})

		test('if fits into schema, test cases are valid', () => {
			return TestCases.getTestCases(testCasesConfigReady).then((testCases) => {
				expect(testCases).toEqual({
					testCases: testCaseList,
					filteredTestCases: testCaseList
				})
			})
		})

		test('if suite does not exist, err is thrown', () => {
			const wrongTestCasesConfigReadyENOENT = new Promise((resolve, reject) => {
				return resolve({
					suites: [
						{
							suite: './test_report/unit/test-cases/suite3',
							config: '',
							tests: {}
						}
					],
					tests: {}
				})
			})
			return expect(TestCases.getTestCases(wrongTestCasesConfigReadyENOENT)).rejects.toThrow(
				"ENOENT: no such file or directory, lstat './test_report/unit/test-cases/suite3'"
			)
		})

		test('if test file is not array, err is thrown', () => {
			jest.spyOn(TestCases, 'importTestCase').mockReturnValue(
				new Promise((resolve, reject) => {
					return resolve({ default: undefined })
				})
			)
			return expect(TestCases.getTestCases(testCasesConfigReady)).rejects.toContain(
				'test case file validation failed'
			)
		})

		test('if test file is an empty array, err is thrown', () => {
			jest.spyOn(TestCases, 'importTestCase').mockReturnValue(
				new Promise((resolve, reject) => {
					return resolve({ default: [] })
				})
			)
			return expect(TestCases.getTestCases(testCasesConfigReady)).rejects.toContain(
				'test case file validation failed'
			)
		})

		test('if test case type is not an object or function, err is thrown', () => {
			jest.spyOn(TestCases, 'importTestCase').mockReturnValue(
				new Promise((resolve, reject) => {
					return resolve({ default: ['string'] })
				})
			)
			return expect(TestCases.getTestCases(testCasesConfigReady)).rejects.toContain(
				'test case file validation failed'
			)
		})

		test('if test case type is null, err is thrown', () => {
			jest.spyOn(TestCases, 'importTestCase').mockReturnValue(
				new Promise((resolve, reject) => {
					return resolve({ default: [null] })
				})
			)
			return expect(TestCases.getTestCases(testCasesConfigReady)).rejects.toContain(
				'test case file validation failed'
			)
		})

		test('if test case type is an array, err is thrown', () => {
			jest.spyOn(TestCases, 'importTestCase').mockReturnValue(
				new Promise((resolve, reject) => {
					return resolve({ default: [[]] })
				})
			)
			return expect(TestCases.getTestCases(testCasesConfigReady)).rejects.toContain(
				'test case file validation failed'
			)
		})

		test('if test case types are different, err is thrown', () => {
			jest.spyOn(TestCases, 'importTestCase').mockReturnValue(
				new Promise((resolve, reject) => {
					return resolve({ default: [() => {}, {}] })
				})
			)
			return expect(TestCases.getTestCases(testCasesConfigReady)).rejects.toContain(
				'test case file validation failed'
			)
		})
	})

	describe('filters', () => {
		test('if name, filtered test cases are valid', () => {
			return TestCases.getTestCases(testCasesConfigReady, ['test1bb', 'test2c.mjs']).then(
				(testCases) => {
					expect(testCases).toEqual({
						testCases: testCaseList,
						filteredTestCases: filteredTestCaseList
					})
				}
			)
		})

		test('if path (in suite), filtered test cases are valid', () => {
			return TestCases.getTestCases(testCasesConfigReady, [
				'testgroup1a/testgroup1b/test1bb',
				'testgroup2a/testgroup2b/testgroup2c/test2c'
			]).then((testCases) => {
				expect(testCases).toEqual({
					testCases: testCaseList,
					filteredTestCases: filteredTestCaseList
				})
			})
		})

		test('if absolute path, filtered test cases are valid', () => {
			return TestCases.getTestCases(testCasesConfigReady, [
				path.join(
					TestEnv.getWorkspacePath(),
					'/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/test1bb'
				),
				path.join(
					TestEnv.getWorkspacePath(),
					'/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c'
				)
			]).then((testCases) => {
				expect(testCases).toEqual({
					testCases: testCaseList,
					filteredTestCases: filteredTestCaseList
				})
			})
		})

		test('if absolute path (workspace), filtered test cases are valid', () => {
			return TestCases.getTestCases(testCasesConfigReady, [
				'/test/e2e/test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/test1bb',
				'/test/e2e/test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c'
			]).then((testCases) => {
				expect(testCases).toEqual({
					testCases: testCaseList,
					filteredTestCases: filteredTestCaseList
				})
			})
		})

		test('if relative path (workspace), filtered test cases are valid', () => {
			return TestCases.getTestCases(testCasesConfigReady, [
				'./test_report/unit/test-cases/suite1/testgroup1a/testgroup1b/test1bb',
				'test_report/unit/test-cases/suite2/testgroup2a/testgroup2b/testgroup2c/test2c'
			]).then((testCases) => {
				expect(testCases).toEqual({
					testCases: testCaseList,
					filteredTestCases: filteredTestCaseList
				})
			})
		})
	})
})

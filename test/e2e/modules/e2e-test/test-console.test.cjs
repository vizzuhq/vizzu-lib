const TestConsole = require('./test-console.cjs')
const TestEnv = require('./test-env.cjs')
const path = require('path')

describe('testSuiteLogPath', () => {
	test('if new TestConsole(), getTestSuiteLogPath() returns undefined', () => {
		const testConsole = new TestConsole()
		expect(testConsole.getTestSuiteLogPath()).toBeUndefined()
	})

	test('if new TestConsole(false), getTestSuiteLogPath() returns undefined', () => {
		const testConsole = new TestConsole(false)
		expect(testConsole.getTestSuiteLogPath()).toBeUndefined()
	})

	test('if new TestConsole(true), getTestSuiteLogPath() returns testSuiteLogPath', () => {
		const testConsole = new TestConsole(true)
		expect(testConsole.getTestSuiteLogPath()).toBe(
			path.join(TestEnv.getTestSuiteReportPath(), 'logs')
		)
	})
})

describe('testStatusPad', () => {
	test('if getTestStatusPad() returns 8', () => {
		const testConsole = new TestConsole()
		expect(testConsole.getTestStatusPad()).toBe(8)
	})

	test('if setTestStatusPad() err is thrown', () => {
		const testConsole = new TestConsole()
		expect(() => {
			testConsole.setTestStatusPad()
		}).toThrow('testStatusPad is integer')
	})

	test('if setTestStatusPad(string) err is thrown', () => {
		const testConsole = new TestConsole()
		expect(() => {
			testConsole.setTestStatusPad('string')
		}).toThrow('testStatusPad is integer')
	})

	test('if setTestStatusPad(9), getTestStatusPad() returns 9', () => {
		const testConsole = new TestConsole()
		testConsole.setTestStatusPad(9)
		expect(testConsole.getTestStatusPad()).toBe(9)
	})
})

describe('testNumberPad', () => {
	test('if getTestNumberPad() returns 0', () => {
		const testConsole = new TestConsole()
		expect(testConsole.getTestNumberPad()).toBe(0)
	})

	test('if setTestNumberPad() err is thrown', () => {
		const testConsole = new TestConsole()
		expect(() => {
			testConsole.setTestNumberPad()
		}).toThrow('testNumberPad is integer')
	})

	test('if setTestNumberPad(string) err is thrown', () => {
		const testConsole = new TestConsole()
		expect(() => {
			testConsole.setTestNumberPad('string')
		}).toThrow('testNumberPad is integer')
	})

	test('if setTestNumberPad(4), getTestNumberPad() returns 4', () => {
		const testConsole = new TestConsole()
		testConsole.setTestNumberPad(4)
		expect(testConsole.getTestNumberPad()).toBe(4)
	})
})

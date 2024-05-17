const TestCasesConfig = require('./test-cases-config.cjs')

const testCasesResult = require('./test-cases-config.test/test-cases-result.cjs')

describe('getConfig()', () => {
	describe('configPathList', () => {
		test('if configPathList is no an array, err is thrown', () => {
			return expect(TestCasesConfig.getConfig()).rejects.toThrow(
				'Assert failed: configPathList is array'
			)
		})

		test('if configPathList is an empty array', () => {
			return TestCasesConfig.getConfig([]).then((config) => {
				expect(config).toEqual({ suites: [], tests: {} })
			})
		})
	})

	describe('configPath', () => {
		test('if configPath name is not string, err is thrown', () => {
			return expect(TestCasesConfig.getConfig([[]])).rejects.toThrow(
				'The "path" argument must be of type string. Received an instance of Array'
			)
		})

		test('if configPath is not file, err is thrown', () => {
			return expect(
				TestCasesConfig.getConfig(['./modules/e2e-test/test-case/test-cases-config.test'])
			).rejects.toThrow('EISDIR: illegal operation on a directory, read')
		})

		test('if configPath does not exist, err is thrown', () => {
			return expect(
				TestCasesConfig.getConfig([
					'./modules/e2e-test/test-case/test-cases-config.test/test-cases-notexist.json'
				])
			).rejects.toThrow('ENOENT: no such file or directory')
		})

		test('if configPath is not json, err is thrown', () => {
			return expect(
				TestCasesConfig.getConfig([
					'./modules/e2e-test/test-case/test-cases-config.test/test-cases-notjson.json'
				])
			).rejects.toThrow('Unexpected end of JSON input')
		})
	})

	describe('config', () => {
		describe('does not fit into schema', () => {
			test('if 1, err is thrown', () => {
				return expect(
					TestCasesConfig.getConfig([
						'./modules/e2e-test/test-case/test-cases-config.test/test-cases-notschema/test-cases-notschema-1.json'
					])
				).rejects.toThrow('Assert failed: config schema validation failed')
			})

			test('if 2, err is thrown', () => {
				return expect(
					TestCasesConfig.getConfig([
						'./modules/e2e-test/test-case/test-cases-config.test/test-cases-notschema/test-cases-notschema-2.json'
					])
				).rejects.toThrow('Assert failed: config schema validation failed')
			})

			test('if 3, err is thrown', () => {
				return expect(
					TestCasesConfig.getConfig([
						'./modules/e2e-test/test-case/test-cases-config.test/test-cases-notschema/test-cases-notschema-3.json'
					])
				).rejects.toThrow('Assert failed: config schema validation failed')
			})

			test('if 4, err is thrown', () => {
				return expect(
					TestCasesConfig.getConfig([
						'./modules/e2e-test/test-case/test-cases-config.test/test-cases-notschema/test-cases-notschema-4.json'
					])
				).rejects.toThrow('Assert failed: config schema validation failed')
			})

			test('if 5, err is thrown', () => {
				return expect(
					TestCasesConfig.getConfig([
						'./modules/e2e-test/test-case/test-cases-config.test/test-cases-notschema/test-cases-notschema-5.json'
					])
				).rejects.toThrow('Assert failed: config schema validation failed')
			})

			test('if 6, err is thrown', () => {
				return expect(
					TestCasesConfig.getConfig([
						'./modules/e2e-test/test-case/test-cases-config.test/test-cases-notschema/test-cases-notschema-6.json'
					])
				).rejects.toThrow('Assert failed: config schema validation failed')
			})

			test('if 7, err is thrown', () => {
				return expect(
					TestCasesConfig.getConfig([
						'./modules/e2e-test/test-case/test-cases-config.test/test-cases-notschema/test-cases-notschema-7.json'
					])
				).rejects.toThrow('Assert failed: config schema validation failed')
			})
		})

		test('if config fits into schema, config is valid', () => {
			return TestCasesConfig.getConfig([
				'./modules/e2e-test/test-case/test-cases-config.test/test-cases-1.json',
				'./modules/e2e-test/test-case/test-cases-config.test/test-cases-2.json',
				'./modules/e2e-test/test-case/test-cases-config.test/test-cases-3.json'
			]).then((config) => {
				config.suites = config.suites.sort((a, b) => {
					if (a.suite < b.suite) return -1
					return a.suite > b.suite ? 1 : 0
				})
				expect(config).toEqual(testCasesResult)
				expect(TestCasesConfig.isTestCasesConfig(config)).toBeTruthy()
			})
		})
	})
})

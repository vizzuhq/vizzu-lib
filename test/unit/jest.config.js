export default {
	rootDir: '../../',
	roots: ['<rootDir>/src', '<rootDir>/test/unit'],
	collectCoverage: true,
	coverageProvider: 'v8',
	coverageDirectory: '<rootDir>/test/unit/.coverage',
	coveragePathIgnorePatterns: ['test/unit/.*'],
	coverageThreshold: {
		global: {
			branches: 100,
			functions: 100,
			lines: 100,
			statements: 100
		}
	},
	testEnvironment: 'jest-environment-node',
	testRegex: '(test/unit/.*(\\.|/)test)\\.js$',
	transform: {}
}

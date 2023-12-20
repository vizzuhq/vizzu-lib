const config = {
	rootDir: '.',
	collectCoverage: true,
	coverageDirectory: '../test_report/unit/.coverage',
	restoreMocks: true,
	testRegex: '(.*test.cjs)$',
	transform: {},
	moduleFileExtensions: ['js', 'cjs'],
	setupFilesAfterEnv: ['jest-extended/all'],
	testTimeout: 10000,
	openHandlesTimeout: 5000
}

module.exports = config

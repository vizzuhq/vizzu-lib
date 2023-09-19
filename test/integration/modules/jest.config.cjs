const config = {
  rootDir: '.',
  collectCoverage: true,
  coverageDirectory: '../test_report/unit/.coverage',
  restoreMocks: true,
  testRegex: '(.*test.js)$',
  transform: {},
  moduleFileExtensions: ['js'],
  setupFilesAfterEnv: ['jest-extended/all']
}

module.exports = config

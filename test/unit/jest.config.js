export default {
  rootDir: '../../',
  roots: ['<rootDir>/src', '<rootDir>/test/unit'],
  collectCoverage: true,
  coverageProvider: 'v8',
  coverageDirectory: '<rootDir>/test/unit/coverage',
  coveragePathIgnorePatterns: ['.test.data.'],
  coverageThreshold: {
    global: {
      branches: 100,
      functions: 100,
      lines: 100,
      statements: 100
    }
  },
  testEnvironment: 'jest-environment-node',
  testRegex: '(test/unit/.*(\\.|/)(test|spec))\\.(jsx?|js?|tsx?|ts?)$',
  transform: {},
  testPathIgnorePatterns: ['<rootDir>/test/unit/node_modules/'],
  moduleFileExtensions: ['js', 'jsx', 'mjs']
}

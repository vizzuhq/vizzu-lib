const TestEnv = require('./test-env.js')
const path = require('path')

const ws = path.join(__dirname, '../../../..')

test('if getWorkspacePath() returns workspacePath', () => {
  expect(TestEnv.getWorkspacePath()).toBe(ws)
})

test('if getTestSuitePath() returns testSuitePath', () => {
  expect(TestEnv.getTestSuitePath()).toBe(path.join(ws, 'test/integration'))
})

test('if getTestSuiteReportPath() returns testSuiteReportPath', () => {
  expect(TestEnv.getTestSuiteReportPath()).toBe(path.join(ws, 'test/integration/test_report'))
})

test('if getTestSuiteResultsPath() returns testSuiteResultsPath', () => {
  expect(TestEnv.getTestSuiteResultsPath()).toBe(
    path.join(ws, 'test/integration/test_report/results')
  )
})

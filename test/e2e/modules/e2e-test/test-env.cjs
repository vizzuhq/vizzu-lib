const path = require('path')

class TestEnv {
	static getWorkspacePath() {
		return path.join(__dirname, '../../../..')
	}

	static getTestSuitePath() {
		return path.join(TestEnv.getWorkspacePath(), TestEnv.getTestSuiteRelativePath())
	}

	static getTestSuiteRelativePath() {
		return 'test/e2e'
	}

	static getTestSuiteReportPath() {
		return path.join(TestEnv.getTestSuitePath(), 'test_report')
	}

	static getTestSuiteResultsPath() {
		return path.join(TestEnv.getTestSuiteReportPath(), 'results')
	}

	static ResultLogTypes = {
		PASSED: 'passed.log',
		WARNINGS: 'warnings.log',
		FAILED: 'failed.log',
		FAILURES: 'failures.log'
	}

	static getTestSuitePassedLog() {
		return path.join(TestEnv.getTestSuiteReportPath(), TestEnv.ResultLogTypes.PASSED)
	}

	static getTestSuiteWarningsLog() {
		return path.join(TestEnv.getTestSuiteReportPath(), TestEnv.ResultLogTypes.WARNINGS)
	}

	static getTestSuiteFailedLog() {
		return path.join(TestEnv.getTestSuiteReportPath(), TestEnv.ResultLogTypes.FAILED)
	}

	static getTestSuiteFailuresLog() {
		return path.join(TestEnv.getTestSuiteReportPath(), TestEnv.ResultLogTypes.FAILURES)
	}
}

module.exports = TestEnv

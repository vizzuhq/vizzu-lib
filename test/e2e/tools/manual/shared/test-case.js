class TestHashStatus {
	static TYPES = {
		ADDED: 'added',
		UPDATED: 'updated',
		UNCHANGED: 'unchanged'
	}
}

class TestCaseResult {
	static TYPES = {
		PASSED: 'PASS',
		WARNING: 'WARN',
		FAILED: 'FAIL',
		UNKNOWN: ''
	}

	static getColor(testResult) {
		switch (testResult) {
			case TestCaseResult.TYPES.PASSED:
				return 'rgba(152, 251, 152, 0.8)'
			case TestCaseResult.TYPES.FAILED:
				return 'rgba(255, 153, 153, 0.8)'
			case TestCaseResult.TYPES.WARNING:
				return 'rgba(255, 255, 153, 0.8)'
			default:
				return ''
		}
	}
}

export { TestHashStatus, TestCaseResult }

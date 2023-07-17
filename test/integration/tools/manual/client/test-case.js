import "../shared/test-case.js"

class TestCase {
    constructor(testConfig, testFile, testIndex, testName, testResult, testSuite, testType) {
      this.testConfig = testConfig;
      this.testFile = testFile;
      this.testIndex = testIndex;
      this.testName = testName;
      this.testResult = testResult;
      this.testSuite = testSuite;
      this.testType = testType;
    }
}

class TestCaseResult {
    static TYPES = window.TestCaseResult.TYPES;
  
    static getColor(testResult) {
      switch (testResult) {
        case TestCaseResult.TYPES.PASSED:
          return "rgba(152, 251, 152, 0.8)";
        case TestCaseResult.TYPES.FAILED:
          return "rgba(255, 153, 153, 0.8)";
        case TestCaseResult.TYPES.WARNING:
          return "rgba(255, 255, 153, 0.8)";
        default:
          return "";
      }
    }
  }

class TestCaseStatus {
  static TYPES = window.TestCaseStatus.TYPES;
}

export { TestCase, TestCaseResult, TestCaseStatus };

class TestCaseStatus {
  static TYPES = {
    ADDED: "added",
    UPDATED: "updated",
    UNCHANGED: "unchanged"
  };
}

class TestCaseResult {
  static TYPES = {
    PASSED: "PASS",
    WARNING: "WARN",
    FAILED: "FAIL",
    UNKNOWN: ""
  };
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = { TestCaseStatus, TestCaseResult };
} else {
  window.TestCaseStatus = TestCaseStatus;
  window.TestCaseResult = TestCaseResult;
}

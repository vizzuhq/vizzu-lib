class TestHashStatus {
  static TYPES = {
    ADDED: "added",
    UPDATED: "updated",
    UNCHANGED: "unchanged",
  };
}

class TestCaseResult {
  static TYPES = {
    PASSED: "PASS",
    WARNING: "WARN",
    FAILED: "FAIL",
    UNKNOWN: "",
  };
}

if (typeof module !== "undefined" && module.exports) {
  module.exports = { TestHashStatus, TestCaseResult };
} else {
  window.TestHashStatus = TestHashStatus;
  window.TestCaseResult = TestCaseResult;
}

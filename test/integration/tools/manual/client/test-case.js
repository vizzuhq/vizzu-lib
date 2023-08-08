import "../shared/test-case.js"

class TestCase {
    constructor(testConfig, testFile, testIndex, testName, testResult, testRepo, testSuite, testType) {
      this.testConfig = testConfig;
      this.testFile = testFile;
      this.testIndex = testIndex;
      this.testName = testName;
      this.testResult = testResult;
      this.testRepo = testRepo;
      this.testSuite = testSuite;
      this.testType = testType;
    }

    static fromJSON(json) {
      return new TestCase(
        json.testConfig,
        json.testFile,
        json.testIndex,
        json.testName,
        json.testResult,
        json.testRepo,
        json.testSuite,
        json.testType
      );
    }

    createOption(select) {
      const option = document.createElement("option");
      option.value =JSON.stringify(this);
      option.setAttribute("name", this.testName);
      this.populateResult(option);
      select.appendChild(option);
    }

    populateResult(option) {
      option.setAttribute("result", this.testResult);
      option.setAttribute("background-color", TestCaseResult.getColor(this.testResult));
      option.textContent = this.testResult ? `${this.testName} | ${this.testResult}` : this.testName;
    }

    static populateStyle(select) {
      select.querySelectorAll("option").forEach((option) => {
        option.style.backgroundColor = option.selected ? "rgba(206,206,206,255)" : option.getAttribute("background-color");
      });
    }

    static validate(select) {
      const testCaseValue = select.value;
      fetch("/validateTestCase", {
        method: "POST",
        headers: {
          "Content-Type": "application/json",
        },
        body: JSON.stringify({ testCaseValue }),
      })
        .then((response) => response.json())
        .then((data) => {
          if (data.status === TestHashStatus.TYPES.UNCHANGED) {
            console.warn(`Hash ${data.status}`);
          } else if (data.status === TestHashStatus.TYPES.ADDED || data.status === TestHashStatus.TYPES.UPDATED) {
            console.log(`Hash ${data.status}`);
            const option = select.options[select.selectedIndex];
            const testCase = TestCase.fromJSON(JSON.parse(option.value))
            testCase.testResult = TestCaseResult.TYPES.PASSED;
            testCase.populateResult(option);
          } else {
            console.error("Hash validation failed:", data.status);
          }
        })
        .catch((error) => {
          console.error("Hash validation failed:", error);
        });
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

class TestHashStatus {
  static TYPES = window.TestHashStatus.TYPES;
}

export default TestCase;

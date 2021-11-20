const TestEnv = require("./test-env.js");
const path = require("path");


test("if getWorkspacePath() returns workspacePath", () => {
    expect(TestEnv.getWorkspacePath()).toBe(path.join(process.cwd(), "../.."));
});

test("if getTestSuitePath() returns testSuitePath", () => {
    expect(TestEnv.getTestSuitePath()).toBe(process.cwd());
});

test("if getTestSuiteReportPath() returns testSuiteReportPath", () => {
    expect(TestEnv.getTestSuiteReportPath()).toBe(path.join(process.cwd(), "test_report"));
});

test("if getTestSuiteResultsPath() returns testSuiteResultsPath", () => {
    expect(TestEnv.getTestSuiteResultsPath()).toBe(path.join(process.cwd(), "test_report", "results"));
});
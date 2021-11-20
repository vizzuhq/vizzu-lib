const TestEnv = require("./test-env.js");
const path = require("path");


test("if getWorkspacePath() is valid", () => {
    expect(TestEnv.getWorkspacePath()).toBe(path.join(process.cwd(), "../.."));
});

test("if getTestSuitePath() is valid", () => {
    expect(TestEnv.getTestSuitePath()).toBe(process.cwd());
});

test("if getTestSuiteReportPath() is valid", () => {
    expect(TestEnv.getTestSuiteReportPath()).toBe(path.join(process.cwd(), "test_report"));
});

test("if getTestSuiteResultsPath() is valid", () => {
    expect(TestEnv.getTestSuiteResultsPath()).toBe(path.join(process.cwd(), "test_report", "results"));
});
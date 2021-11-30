const path = require("path");


class TestEnv {

    static getWorkspacePath() {
        return path.join(__dirname, "../../../..");
    }


    static getTestSuitePath() {
        return path.join(TestEnv.getWorkspacePath(), "test", "integration");
    }


    static getTestSuiteReportPath() {
        return path.join(TestEnv.getTestSuitePath(), "test_report");
    }


    static getTestSuiteResultsPath() {
        return path.join(TestEnv.getTestSuiteReportPath(), "results");
    }
}


module.exports = TestEnv;
